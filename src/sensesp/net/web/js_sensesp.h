#include <pgmspace.h>
const char PAGE_js_sensesp[] PROGMEM = R"=====(
function ajax(method,url,data,contentType){return new Promise((function(resolve,reject){var request=new XMLHttpRequest;request.open(method,url,!0),request.onload=function(){200===request.status?resolve(request.response):reject(Error(request.statusText))},request.onerror=function(){reject(Error("Network Error"))},contentType&&request.setRequestHeader("Content-Type",contentType),request.send(data)}))}class TreeList{constructor(main,pathList){this.main=main,this.main.appendChild(document.createElement("div")),this.root=document.createElement("ul"),this.root.id="tree",this.main.appendChild(this.root);for(var i=0;i<pathList.length;i++){var entry=pathList[i],fullPath=entry,parts=entry.split("/"),nodeName=parts.pop(),section=this.main;for(parts.shift();parts.length>0;){var sectionName=parts.shift();section=this.findNode(section,sectionName)}this.addEntry(section,nodeName,fullPath)}var toggler=document.getElementsByClassName("caret");for(i=0;i<toggler.length;i++)toggler[i].addEventListener("click",(function(){this.parentElement.querySelector(".nested").classList.toggle("active"),this.classList.toggle("caret-down")}))}makeSectionNode(name){var section=document.createElement("li"),caret=document.createElement("span");caret.className="caret",caret.innerHTML=name,section.appendChild(caret);var ul=document.createElement("ul");return ul.className="nested",section.appendChild(ul),section}addEntry(section,name,fullPath){var entry=document.createElement("li");entry.innerHTML=name,section.children[1].appendChild(entry),entry.className="selectable",entry.addEventListener("click",(function(){editConfig(fullPath)}))}findNode(sectionRoot,nodeName){if(""!=nodeName){var i,nextEntry,searchChildren=sectionRoot.children[1].childNodes;for(i=0;i<searchChildren.length;i++)if((nextEntry=searchChildren[i]).childNodes[0].textContent==nodeName)return nextEntry;return nextEntry=this.makeSectionNode(nodeName),sectionRoot.children[1].appendChild(nextEntry),nextEntry}return sectionRoot}}var globalEditor=null;function getEmptyMountDiv(){var main=document.getElementById("mountNode");return main.empty(),globalEditor=null,main}function editConfig(config_path){var main=getEmptyMountDiv();ajax("GET","/config"+config_path).then((response=>{var json=JSON.parse(response),config=json.config,schema=json.schema;if(0==Object.keys(schema).length)return alert("No schema available for "+config_path),void showConfigTree();schema.title||(schema.title="Configuration for "+config_path),main.innerHTML="\n        <div class='row'>\n        <div id='editor_holder' class='medium-12 columns'></div>        \n        </div>\n        <div class='row'>\n        <div class='medium-12-columns'>\n        <button id='submit' class='tiny'>Save</button>\n        <button id='cancel' class='tiny'>Cancel</button>\n        <span id='valid_indicator' class='label'></span>\n        </div>\n        </div>\n      ",globalEditor=new JSONEditor(document.getElementById("editor_holder"),{schema:schema,startval:config,no_additional_properties:!0,disable_collapse:!0,disable_properties:!0,disable_edit_json:!0,show_opt_in:!0}),document.getElementById("submit").addEventListener("click",(function(){saveConfig(config_path,globalEditor.getValue())})),document.getElementById("cancel").addEventListener("click",(function(){showConfigTree()})),globalEditor.on("change",(function(){var errors=globalEditor.validate(),indicator=document.getElementById("valid_indicator");errors.length?(indicator.className="label alert",indicator.textContent="not valid"):(indicator.className="label success",indicator.textContent="valid")}))})).catch((err=>{alert(`Error retrieving configuration ${config_path}: ${err.message}`),showConfigTree()}))}function saveConfig(config_path,values){ajax("PUT","/config"+config_path,JSON.stringify(values),"application/json").then((response=>{showConfigTree()})).catch((err=>{alert(`Error saving configuration ${config_path}: ${err.message}`),showConfigTree()}))}function showConfigTree(){var main=getEmptyMountDiv();ajax("GET","/config").then((response=>{var configList=JSON.parse(response).keys;return configList.sort(),configList})).then((configList=>{new TreeList(main,configList)})).catch((err=>{alert("Error: "+err.statusText)}))}Element.prototype.empty=function(){for(var child=this.lastElementChild;child;)this.removeChild(child),child=this.lastElementChild};
)=====";