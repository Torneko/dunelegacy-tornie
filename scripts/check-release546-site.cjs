const {chromium}=require('playwright'),fs=require('fs');
(async()=>{
 const bases=process.argv.slice(2);if(!bases.length)throw Error('Provide one or more guide URLs');
 const browser=await chromium.launch({executablePath:process.env.SITE_BROWSER,headless:true,args:['--no-sandbox','--disable-dev-shm-usage']});
 const results=[],errors=[];
 for(const base of bases)for(const lang of ['fr','en'])for(const width of [1440,390]){
  const page=await browser.newPage({viewport:{width,height:960}});page.on('pageerror',e=>errors.push(String(e)));
  for(const mod of ['Tornie','TornieLite','Jericho','JerichoLite','ArcaKev','ArcaKevLite','Vanilla']){
   await page.goto(base+(lang==='fr'?'index.html':'index-en.html')+'#factions?mod='+mod+'&house=A');
   const result=await page.evaluate(({mod})=>{
    const d=window.SITE_DATA,revised=['Tornie','TornieLite'].includes(mod);
    const section=[...document.querySelectorAll('main .campaign-plan')].find(s=>s.querySelector('h2')?.textContent.includes('1.0.546'));
    if(d.version!=='1.0.546'||d.rules546?.protocol!==24||d.rules546?.saveVersion!==9839)throw Error('Release metadata differs');
    if(Boolean(section)!==revised)throw Error('Campaign revision shown for wrong edition '+mod);
    if(revised){
     const rows=[...section.querySelectorAll('tbody tr')].map(row=>[...row.children].map(c=>c.textContent.trim()));
     const expected=d.campaignPlans[mod].map(row=>[row.house,row.persistentSupport]);
     if(JSON.stringify(rows)!==JSON.stringify(expected))throw Error('Support table differs '+mod);
     if(mod==='Tornie'&&!rows.some(([house,support])=>house==='Rebels'&&support==='Harkonnen'))throw Error('Rebels support differs');
    }
    for(const [name,level] of [['Hightech Factory',4],['Carryall',4],['Chemical Carryall',5]]){
     const item=[...d.units,...d.buildings].find(item=>item.name===name);
     if(!item||item.routes.some(route=>route.house==='W'&&typeof route.level==='number'&&route.level!==level))throw Error('Wildspade technology differs '+name);
    }
    if(document.documentElement.scrollWidth>innerWidth+2)throw Error('Page overflow '+mod);
    return {mod,revised,rows:section?.querySelectorAll('tbody tr').length||0};
   },{mod});
   results.push({base,lang,width,...result});
  }
  await page.close();
 }
 if(errors.length)throw Error(errors.join('\n'));
 fs.writeFileSync(process.env.RELEASE546_SITE_OUTPUT||'reports/website-546-campaigns.json',JSON.stringify({passed:true,count:results.length,results},null,2));
 await browser.close();console.log('PASS: '+results.length+' FR/EN campaign scope, support tables and Wildspade technology contexts');
})().catch(e=>{console.error(e);process.exit(1)});
