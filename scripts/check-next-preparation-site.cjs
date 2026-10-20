// Actual browser checks for the unpublished post-544 documentation preview.
const {chromium}=require('playwright'),fs=require('fs');
(async()=>{
 const base=process.argv[2];if(!base)throw Error('Provide preview URL');
 const browser=await chromium.launch({executablePath:process.env.SITE_BROWSER||'/usr/bin/chromium',headless:true,args:['--no-sandbox','--disable-dev-shm-usage']});
 const results=[],errors=[];
 const rosters={Vanilla:'',Tornie:'HAOFSMNRCWKT',TornieLite:'HAOFSM',Jericho:'HAOFSMWKTNRC',JerichoLite:'HAOFSM',ArcaKev:'HAOFSMKWRNTC',ArcaKevLite:'HAOFSM'};
 for(const lang of ['fr','en'])for(const width of [1440,390]){
  const page=await browser.newPage({viewport:{width,height:960}});page.on('pageerror',e=>errors.push(String(e)));
  for(const [mod,roster] of Object.entries(rosters))for(const route of ['technologies','factions','batiments']){
   await page.goto(base+(lang==='fr'?'index.html':'index-en.html')+'#'+route+'?mod='+mod+'&house=A'+(route==='batiments'?'&item=radar':''));
   const root=route==='batiments'?page.locator('dialog[open]'):page.locator('main');
   await root.waitFor();
   const rows=await root.locator('[data-research-house]').evaluateAll(a=>a.map(e=>e.dataset.researchHouse).join(''));
   if(rows!==roster)throw Error(JSON.stringify({lang,width,mod,route,rows,roster}));
   if(mod!=='Vanilla'){
    const text=await root.locator('.next-research').innerText();
    if(!['500','45','750','60','IX','+2'].every(x=>text.includes(x)))throw Error('Research parameters missing');
   }
   if(await page.evaluate(()=>document.documentElement.scrollWidth>innerWidth+2))throw Error('Page overflow');
   if(route==='batiments' && width===390 && mod==='ArcaKev')await page.screenshot({path:(process.env.NEXT_SITE_OUTPUT||'.')+'/radar-preview-'+lang+'.png'});
   results.push({lang,width,mod,route,roster});
  }
  await page.close();
 }
 if(errors.length)throw Error(errors.join('\n'));
 const out=process.env.NEXT_SITE_OUTPUT||'.';fs.writeFileSync(out+'/next-website-validation.json',JSON.stringify({passed:true,count:results.length,results,errors},null,2));
 console.log('PASS: '+results.length+' research/technology/faction contexts, FR/EN desktop/mobile; correct roster order, prerequisites and Vanilla gate');
 await browser.close();
})().catch(e=>{console.error(e);process.exit(1)});
