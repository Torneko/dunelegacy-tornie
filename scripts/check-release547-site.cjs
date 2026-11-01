const {chromium}=require('playwright'),fs=require('fs');
(async()=>{
 const base=process.argv[2];if(!base)throw Error('Provide guide URL');
 const browser=await chromium.launch({executablePath:process.env.SITE_BROWSER,headless:true,args:['--no-sandbox']});
 const checks=[],errors=[];
 for(const lang of ['fr','en'])for(const width of [1440,390]){
  const page=await browser.newPage({viewport:{width,height:960}});page.on('pageerror',e=>errors.push(String(e)));
  for(const mod of ['Tornie','Jericho','ArcaKev']){
   await page.goto(base+(lang==='fr'?'index.html':'index-en.html')+'#unites?mod='+mod+'&house=K&item=super-tank');
   await page.waitForSelector('dialog[open]');
   const proof=await page.evaluate(mod=>{
    const d=window.SITE_DATA,unit=d.units.find(u=>u.id==='super-tank'),old=d.units.find(u=>u.id==='elite-siege-tank');
    if(d.version!=='1.0.547'||d.rules547.protocol!==25||d.rules547.saveVersion!==9840)throw Error('547 metadata');
    if(unit.routes.length!==3||unit.routes.some(r=>r.house!=='K'||r.price!==925||r.level!==7||r.upgrade!==0||!r.prerequisites.includes('House IX')))throw Error('Super Tank routes');
    if(old.routes.some(r=>r.house==='K'))throw Error('Kleshmersh retains Elite Siege Tank route');
    const text=document.querySelector('dialog').textContent;
    if(!text.includes('925')||!text.includes('300')||!text.includes('Kleshmersh'))throw Error('Super Tank facts missing');
    if(!d.rules547.editorWindtraps.greenStar||d.rules547.editorWindtraps.chaosRequired)throw Error('Windtrap editor metadata');
    return {mod,routes:unit.routes.length};
   },mod);
   await page.evaluate(async()=>{await Promise.all([...document.images].map(i=>{i.loading='eager';return i.decode().catch(()=>{});}));});
   if(await page.evaluate(()=>document.documentElement.scrollWidth>innerWidth+2||[...document.querySelectorAll('dialog img')].some(i=>!i.complete||!i.naturalWidth)))throw Error('Super Tank artwork or layout');
   checks.push({lang,width,...proof});
  }
  await page.goto(base+(lang==='fr'?'index.html':'index-en.html'));
  if(width===390)await page.locator('.menu-toggle').click();
  if(!await page.locator('.official-site a').first().isVisible())throw Error('DuneCity link missing');
  checks.push({lang,width,officialLinks:true});await page.close();
 }
 await browser.close();if(errors.length)throw Error(errors.join('\n'));
 fs.writeFileSync(process.env.RELEASE547_SITE_OUTPUT||'reports/website-547.json',JSON.stringify({passed:true,count:checks.length,checks,errors},null,2));
 console.log('PASS: '+checks.length+' FR/EN Super Tank, faction replacement, price, technology, artwork and DuneCity contexts');
})().catch(e=>{console.error(e);process.exit(1)});
