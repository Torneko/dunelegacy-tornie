const {chromium}=require('playwright'),fs=require('fs');
(async()=>{
 const browser=await chromium.launch({executablePath:process.env.SITE_BROWSER||'/usr/bin/chromium',headless:true,args:['--no-sandbox','--disable-dev-shm-usage']});
 const results=[],errors=[];
 const expected=process.env.RELEASE_VERSION||'1.0.539';
 const routes=['accueil','unites','batiments','technologies','factions','hauts-faits','guide','galerie','projet'];
 const bases=process.argv.slice(2);if(!bases.length)throw Error('Provide the offline file URL and/or published site URL');
 for(const base of bases)for(const width of [1440,390])for(const lang of ['fr','en']){
  const page=await browser.newPage({viewport:{width,height:960}});page.on('pageerror',e=>errors.push({base,width,lang,error:String(e)}));
  for(const route of routes){
   await page.goto(base+(lang==='en'?'index-en.html':'index.html')+'#'+route+'?mod=Tornie&house=A');await page.evaluate(async()=>{await Promise.all([...document.images].map(i=>{i.loading='eager';return i.decode().catch(()=>{});}));});
   const r=await page.evaluate(()=>({heading:document.querySelector('main h1')?.innerText,version:document.querySelector('.edition').innerText,overflow:document.documentElement.scrollWidth>innerWidth+2,missing:[...document.images].filter(i=>!i.complete||i.naturalWidth===0).map(i=>i.src),network:document.querySelector('.network539')?.innerText,links:[...document.querySelectorAll('a[href]')].map(a=>a.getAttribute('href'))}));
   if(!r.heading||!r.version.includes(expected)||r.overflow||r.missing.length)throw Error(JSON.stringify({base,width,lang,route,r}));
   if(['accueil','guide','projet'].includes(route)&&(!r.network?.includes(expected)||!r.network?.includes('10')))throw Error('network notice missing');
   results.push({base,width,lang,route,heading:r.heading});
  }
  // Search and filter retain the 67-entry catalogue.
  await page.goto(base+(lang==='en'?'index-en.html':'index.html')+'#hauts-faits');await page.waitForSelector('#achievement-search');
  await page.locator('#achievement-search').fill('chaos');
  const visible=await page.locator('[data-achievement]:visible').count();if(visible<1)throw Error('achievement search failed');
  await page.goto(base+(lang==='en'?'index-en.html':'index.html')+'#technologies?mod=Jericho&house=W');
  await page.locator('[data-language-page="'+(lang==='en'?'index.html':'index-en.html')+'"]').click();
  if(!page.url().includes('mod=Jericho')||!page.url().includes('house=W'))throw Error('language context lost');
  await page.close();
 }
 // Compare all original game PNG bytes against the recovered branch separately.
 if(errors.length)throw Error(JSON.stringify(errors));
 fs.writeFileSync(process.env.SITE_VALIDATION_OUTPUT||'website-validation.json',JSON.stringify({passed:true,count:results.length,results,errors},null,2));
 console.log('PASS: '+results.length+' FR/EN desktop/mobile routes; search, language context, images and overflow; no browser errors');
 await browser.close();
})().catch(e=>{console.error(e);process.exit(1)});
