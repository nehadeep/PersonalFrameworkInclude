document.onmouseover=autoClear;document.onclick=hideAllMenus;window.onload=cmCustomAddEventAfter(window,"onload",window.onload,"initializeHacks()",initializeHacks);window.onerror=null;var timeOn=null;var activeMenu=null;var offsetArrayX=new Array;var offsetArrayY=new Array;var CrossBrowserMenus_InitalizationFunctions=new Array;var numMenus=DDMMenuCount;if(numMenus==null){numMenus=0}function autoClear(){if(!document.getElementById){return }menuActive=0;if(timeOn==null){timeOn=setTimeout("hideAllMenus()",400)}}function menuOver(){if(!document.getElementById){return }clearTimeout(timeOn);timeOn=null;menuActive=1}function menuOut(){if(!document.getElementById){return }if(timeOn==null){timeOn=setTimeout("hideAllMenus()",400)}}function getStyleObject(objectId){if(document.getElementById&&document.getElementById(objectId)){return document.getElementById(objectId).style}else{if(document.all&&document.all(objectId)){return document.all(objectId).style}else{if(document.layers&&document.layers[objectId]){return document.layers[objectId]}else{return false}}}}function changeObjectVisibility(objectId,newVisibility){var styleObject=getStyleObject(objectId);if(styleObject){styleObject.visibility=newVisibility;styleObject.overflow=newVisibility;return true}else{return false}}function getOffsetX(element,menuNumber){return(element.offsetLeft+getParentOffsetX(element.offsetParent)-element.offsetParent.offsetLeft)}function getParentOffsetX(element){if(element.offsetParent!=null){return(element.offsetLeft+getParentOffsetX(element.offsetParent))}else{return(0)}}function getOffsetY(element,menuNumber){return(element.offsetHeight+element.offsetTop+getParentOffsetY(element.offsetParent))}function getParentOffsetY(element){if(element.offsetParent!=null){return(element.offsetTop+getParentOffsetY(element.offsetParent))}else{return(0)}}function showMenu(menuNumber,eventObj,labelID){if(document.getElementById==null){return }activeMenu=document.getElementById("DDMMenu_"+menuNumber);if(activeMenu==null||document.getElementById("DDMMenu_"+menuNumber).style.visibility!="hidden"){return }clearTimeout(timeOn);timeOn=null;hideAllMenus(menuNumber);document.getElementById("DDMMenu_"+menuNumber).style.top=0;document.getElementById("DDMMenu_"+menuNumber).style.left=0;document.getElementById("DDMMenu_"+menuNumber).style.left=getOffsetX(document.getElementById("DDMAnchor_"+menuNumber),menuNumber);document.getElementById("DDMMenu_"+menuNumber).style.top=getOffsetY(document.getElementById("DDMAnchor_"+menuNumber),menuNumber);eventObj.cancelBubble=true;hideSelect();var menuId="DDMMenu_"+menuNumber;if(changeObjectVisibility(menuId,"visible")){return true}else{return false}}function hideAllMenus(menuNumber){for(counter=1;counter<=numMenus;counter++){if(counter!=menuNumber){changeObjectVisibility("DDMMenu_"+counter,"hidden")}}showSelect()}function initializeHacks(){if((navigator.appVersion.indexOf("MSIE 5")!=-1)&&(navigator.platform.indexOf("Mac")!=-1)&&getStyleObject("blankDiv")){window.onresize=explorerMacResizeFix}resizeBlankDiv();createFakeEventObj();DDM_IsLoaded=true;CrossBrowserMenus_ExecuteInitializationFunctions()}function createFakeEventObj(){if(!window.event){window.event=false}}function resizeBlankDiv(){if((navigator.appVersion.indexOf("MSIE 5")!=-1)&&(navigator.platform.indexOf("Mac")!=-1)&&getStyleObject("blankDiv")){getStyleObject("blankDiv").width=document.body.clientWidth-20;getStyleObject("blankDiv").height=document.body.clientHeight-20}}function explorerMacResizeFix(){location.reload(false)}function showSelect(){var obj;if(document.all){for(var i=0;i<document.all.tags("select").length;i++){obj=document.all.tags("select")[i];if(!obj||!obj.offsetParent){continue}obj.style.visibility="visible";obj.style.overflow="visible"}}}function hideSelect(){var obj;var currentEle;var top=0;var left=0;var menuHeight;var timeout;if(document.all){for(var i=0;i<document.all.tags("select").length;i++){obj=document.all.tags("select")[i];currentEle=obj;while(currentEle.tagName.toLowerCase()!="body"){top+=currentEle.offsetTop;left+=currentEle.offsetLeft;currentEle=currentEle.offsetParent}if(activeMenu!=null){menuHeight=(activeMenu.offsetTop+activeMenu.offsetHeight);if(top<menuHeight){if((left<(activeMenu.offsetLeft+activeMenu.offsetWidth))&&(left+obj.offsetWidth>activeMenu.offsetLeft)){obj.style.visibility="hidden"}obj.style.overflow="hidden"}}top=0;left=0}}}function CrossBrowserMenus_AddInitalizationFunction(functionName){CrossBrowserMenus_InitalizationFunctions[CrossBrowserMenus_InitalizationFunctions.length]=functionName}function CrossBrowserMenus_ExecuteInitializationFunctions(){for(i=0;i<CrossBrowserMenus_InitalizationFunctions.length;i++){eval(CrossBrowserMenus_InitalizationFunctions[i])}};