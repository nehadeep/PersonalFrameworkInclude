//v3.0
var BaynoteJSVersion="$Revision: 1.27 $";var BN_READY_SIGNAL="ReadySignal";if(typeof(baynote_globals)=="undefined")var baynote_globals=new Object();baynote_globals.waitForReady=false;function BNLog(){this.timeBase=new Date().getTime();this.lines=new Array();this.lastLine="";this.repCount=0;}
BNLog.prototype.log=function(str){if(str==this.lastLine){++this.repCount;return;}
if(this.repCount>0){this.lines.push("___ ABOVE REPEATED "+this.repCount+" TIME"+((this.repCount>1)?"S":""));}
this.lastLine=str;this.repCount=0;var elapsed=new Date().getTime()-this.timeBase
this.lines.push(elapsed+": "+str);}
BNLog.prototype.toString=function(){if(this.repCount>0){this.lines.push("___ ABOVE REPEATED "+this.repCount+" TIME"
+((this.repCount>1)?"S":""));this.lastLine="";this.repCount=0;}
return this.lines.join("\n");}
if(typeof(bnLog)=="undefined"){var bnLog=new BNLog();}
function BNCriticalSectionQueue(){this.waitList=new Object();this.lastId=0;}
BNCriticalSectionQueue.prototype.issueId=function(){return++this.lastId;}
BNCriticalSectionQueue.prototype.enqueue=function(id,item){this.waitList[id]=item;}
BNCriticalSectionQueue.prototype.getWaiter=function(id){return(id==null)?null:this.waitList[id];}
BNCriticalSectionQueue.prototype.firstWaiter=function(){return this.getWaiter(this.nextWaiterKeyAfter(null));}
BNCriticalSectionQueue.prototype.nextWaiterAfter=function(id){return this.getWaiter(this.nextWaiterKeyAfter(id));}
BNCriticalSectionQueue.prototype.nextWaiterKeyAfter=function(id){for(var currKey in this.waitList){if(typeof(this.waitList[currKey])!="object")continue;if(id==null)return currKey;if(id==currKey)id=null;}
return null;}
BNCriticalSectionQueue.prototype.nextPredecessor=function(target,start){for(var currWaiter=start;currWaiter!=null;currWaiter=this.nextWaiterAfter(currWaiter.id)){if(currWaiter.enter||(currWaiter.number!=0&&(currWaiter.number<target.number||(currWaiter.number==target.number&&currWaiter.id<target.id)))){return currWaiter;}}
return null;}
function BNCriticalSection(csQueue){this.csQueue=csQueue;this.debug=1;}
BNCriticalSection.prototype.enter=function(enterFunc){this.enterFunc=enterFunc;this.id=this.csQueue.issueId();this.csQueue.enqueue(this.id,this);this.enter=true;this.number=(new Date()).getTime();this.enter=false;this.attempt(this.csQueue.firstWaiter());}
BNCriticalSection.prototype.leave=function(){if(this.debug)bnLog.log("LEAVE "+this.id);this.number=0;}
BNCriticalSection.prototype.attempt=function(start){var nextReady=this.csQueue.nextPredecessor(this,start);if(nextReady!=null){if(this.debug)bnLog.log("WAIT "+this.id);var me=this;return setTimeout(function(){me.attempt(nextReady);},50);}
if(this.debug)bnLog.log("ENTER "+this.id);this.enterFunc();}
function BNResourceManager(){this.csQueue=new BNCriticalSectionQueue();this.critSec=null;this.debug=1;this.resources=new Object();this.waiting=new Object();}
BNResourceManager.prototype.getResource=function(rId){return this.resources[rId];}
BNResourceManager.prototype.loadResource=function(rId,rAddress,rType){if(typeof(this.resources[rId])!="undefined")return;this.resources[rId]=null;var critSec=new BNCriticalSection(this.csQueue);critSec.enter(function(){bnResourceManager.inject(rId,rAddress,rType,critSec);});}
BNResourceManager.prototype.inject=function(rId,rAddress,rType,critSec){this.critSec=critSec;if(this.debug)bnLog.log("INJECT "+this.critSec.id+" ("+rId+")");if(!rType||rType=="script"){var scriptTag1=document.createElement("script");scriptTag1.language="javascript";scriptTag1.src=rAddress;var head=document.getElementsByTagName("head");head[0].appendChild(scriptTag1);}
else if(rType=="img"){var img=document.createElement("IMG");var handler=function(){bnResourceManager.registerAndAddResource(rId,img);};if(img.addEventListener)img.addEventListener("load",handler,false);else if(img.attachEvent)img.attachEvent("onload",handler);else img["onload"]=handler;img.src=rAddress;img.style.display="none";var ph=document.getElementById("bn_placeholder_global");if(ph!=null)ph.appendChild(img);}
else alert("Unexpected resource type to loadResource: "+rType);}
BNResourceManager.prototype.waitForResource=function(rId,callbackCode,rAddress,rType){with(this){if(getResource(rId)){this.runCallback(callbackCode);}
else{if(typeof(waiting[rId])=="undefined")waiting[rId]=new Array();var waitingList=waiting[rId];waitingList[waitingList.length]=callbackCode;if(rAddress)this.loadResource(rId,rAddress,rType);}}}
BNResourceManager.prototype.wakeUpWaiting=function(rId){with(this){var waitingList=waiting[rId];if(!waitingList)return;for(var i=0;i<waitingList.length;i++){if(waitingList[i]){var codeToEval=waitingList[i];waitingList[i]=null;if(this.debug&&codeToEval)bnLog.log("CALLBACK "+rId+": "+codeToEval);this.runCallback(codeToEval);}}}}
BNResourceManager.prototype.registerAndAddResource=function(rId,resource){if(this.debug)bnLog.log("REGISTER "+(this.critSec?this.critSec.id:"")+" ("+rId+")");this.resources[rId]=resource;this.wakeUpWaiting(rId);this.critSec.leave();setTimeout("bnResourceManager.wakeUpWaiting('"+rId+"')",5000);}
BNResourceManager.prototype.registerResource=function(rId){this.registerAndAddResource(rId,true);}
BNResourceManager.prototype.runCallback=function(callback){if(typeof(callback)=="string")eval(callback);else if(typeof(callback)=="function")callback();else alert("Invalid callback, type="+typeof(callback));}
if(typeof(bnResourceManager)=="undefined"){var bnResourceManager=new BNResourceManager();}
function BNSystem(){this.testServer=null;}
BNSystem.prototype.getCookieValue=function(cookieName,cookieSubDomain){if(!cookieSubDomain)cookieSubDomain=baynote_globals.cookieSubDomain;if(cookieSubDomain)cookieName+=("-"+cookieSubDomain);var sRE="(?:; )?"+cookieName+"=([^;]*);?";var oRE=new RegExp(sRE);if(oRE.test(document.cookie)){return decodeURIComponent(RegExp["$1"]);}else{return null;}}
BNSystem.prototype.setCookie=function(cookieName,cookieValue,cookiePath,cookieExpires,cookieDomain,cookieSubDomain){cookieValue=encodeURIComponent(cookieValue);if(cookieExpires=="NEVER"){var nowDate=new Date();nowDate.setFullYear(nowDate.getFullYear()+500);cookieExpires=nowDate.toGMTString();}
else if(cookieExpires=="SESSION")cookieExpires="";if(cookiePath!="")cookiePath=";Path="+cookiePath;if(cookieExpires!="")cookieExpires=";expires="+cookieExpires;if(!cookieDomain)cookieDomain=(baynote_globals.cookieDomain)?baynote_globals.cookieDomain:"";if(cookieDomain!="")cookieDomain=";domain="+cookieDomain;if(!cookieSubDomain)cookieSubDomain=baynote_globals.cookieSubDomain;if(cookieSubDomain)cookieName+=("-"+cookieSubDomain);var cookieStr=cookieName+"="+cookieValue+cookieExpires+cookiePath+cookieDomain;if(cookieStr.length>4096)return false;document.cookie=cookieStr;return true;}
BNSystem.prototype.removeCookie=function(cookieName,cookieDomain){this.setCookie(cookieName,"","/","Mon, 1 Jan 1990 00:00:00",cookieDomain);}
BNSystem.prototype.getURLParam=function(name,url){if(!url)var url=window.location.href;var regex=new RegExp("[\\?&]"+name+"=([^&#]*)");var match=regex.exec(url);if(!match)return null;else return match[1];}
BNSystem.prototype.getTestServer=function(){if(this.testServer!=null)return this.testServer;var testServer=this.getURLParam("bn_test");if(testServer)this.setCookie("bn_test",testServer,"/","SESSION");else if(testServer=="")this.removeCookie("bn_test");else{testServer=this.getCookieValue("bn_test");if(!testServer)testServer="";}
this.testServer=testServer;return testServer;}
if(typeof(bnSystem)=="undefined"){var bnSystem=new BNSystem();}
if(typeof(BNTag)=="undefined"){function BNTag(previousTag){if(previousTag){this.id=previousTag.id+1;this.server=previousTag.server;this.customerId=previousTag.customerId;this.code=previousTag.code;}
else this.id=0;this.attrs=new Object();this.docAttrs=new Object();this.css=new Object();}}
BNTag.prototype.getCommonResourceId=function(){return"Common";}
BNTag.prototype.getCommonResourceAddress=function(tag){return(this.server+"/baynote/tags2/common.js");}
BNTag.prototype.getFailsafeResourceId=function(){return"Failsafe";}
BNTag.prototype.getFailsafeResourceAddress=function(){var v=BaynoteJSVersion.split(" ")[1];var u=bnSystem.getCookieValue("bn_u");return(this.server+"/baynote/customerstatus2?customerId="+this.customerId+"&code="+this.code+"&x="+this.id+(new Date().getTime())+"&v="+v+"&u="+u);}
BNTag.prototype.show=function(parentElemId){if(this.id==0)document.write("<span id='bn_placeholder_global'></span>");this.placeHolderId="bn_placeholder"+this.id;var placeHolderType;if(this.placeHolderElement)placeHolderType=this.placeHolderElement;else placeHolderType=this.popup?"span":"div";if(parentElemId){var placeHolder=document.createElement(placeHolderType);placeHolder.id=this.placeholderId;document.getElementById(parentElemId).appendChild(placeHolder);}
else document.write("<"+placeHolderType+" id='"+this.placeHolderId+"'></"+placeHolderType+">");window["bn_tags"][this.id]=this;var testServer=bnSystem.getTestServer();if(testServer){var reValidTestServer=new RegExp("^https?://[^/]*\.baynote\.(com|net)(:\d+)?(/.*)?");if(reValidTestServer.test(testServer))this.server=testServer;else alert("Ignoring invalid test server \""+testServer+"\"");}
this.showWhenReady(this);baynote_tag=new BNTag(this);}
BNTag.prototype.showWhenReady=function(tag){if(baynote_globals.waitForReady&&!bnResourceManager.getResource(BN_READY_SIGNAL)){bnResourceManager.waitForResource(BN_READY_SIGNAL,function(){tag.showWhenReady(tag);});return;}
var commonId=this.getCommonResourceId();if(!bnResourceManager.getResource(commonId)){bnResourceManager.waitForResource(commonId,function(){tag.showWhenReady(tag);},this.getCommonResourceAddress(),"script");return;}
bnTagManager.show(tag.id);}
BNTag.prototype.noshow=function(){window["bn_tags"][this.id]=this;baynote_tag=new BNTag(this);}
BNTag.prototype.getParam=function(name,defaultValue){var value=this[name];if(typeof(value)=="undefined"||value==null)return defaultValue;else return value;}
if(typeof(baynote_tag)=="undefined"){window["bn_tags"]=new Array();var baynote_tag=new BNTag(null);}
function bnReadySignal(){bnResourceManager.registerResource(BN_READY_SIGNAL);}
function bnCall(resName,methodName,methodArg){var resource=bnResourceManager.getResource(resName);if(!resource){bnResourceManager.waitForResource(resName,function(){bnCall(resName,methodName,methodArg);});return;}
if(typeof(resource)!="object"){return;}
var method=resource[methodName];if(typeof(method)!="function"){return;}
method.call(resource,methodArg);}
function bn_showObserver() {
	var bn_location_href = window.location.href;
	if (bn_location_href.indexOf("https://") == 0) {
		baynote_tag.server = "https://cdw-www.baynote.net";
	} else {
		baynote_tag.server = "http://cdw-www.baynote.net";
	}
	baynote_tag.customerId = "cdw";
	baynote_tag.code = "www";
	baynote_tag.type = "baynoteObserver";

	if (bn_location_href.search(/^https?:\/\/[^\/]*cdwg.com/)> -1) {
		baynote_globals.cookieDomain = "cdwg.com";
	} else if (bn_location_href.search(/^https?:\/\/[^\/]*cdw.ca/)> -1) {
		baynote_globals.cookieDomain = "cdw.ca";
	}
	else {
		baynote_globals.cookieDomain = "cdw.com";
	}

	baynote_tag.exitConfirmation = bn_onClickHandler;
	baynote_getOrderInfo();
	baynote_tag.show();
}

function bn_waitForPolicy() {
	if(typeof(bnResourceManager) != "undefined" && bnResourceManager != null) {
		bnResourceManager.waitForResource("Policy", function() { bn_checkPolicy(); });
	}
}

//create global variable for guide container prefix
var bn_gContainerPreifx;
//For hiding Recommendations
function bn_checkPolicy() {
var bn_location_href = window.location.href;
	if (!bnPolicy.get("guide","ok")) {
		//setting test cookie
		if (bn_location_href.search(/^https?:\/\/[^\/]*cdwg.com/)> -1) {
				bn_setCookie('bn_cdw_ab','false','cdwg.com',90);
				} else if (bn_location_href.search(/^https?:\/\/[^\/]*cdw.ca/)> -1) {
					bn_setCookie('bn_cdw_ab','false','cdw.ca',90);
				} else {
				bn_setCookie('bn_cdw_ab','false','cdw.com',90);
				}
		if(document.getElementById("RA_pnlStatic")) {
		      (document.getElementById("RA_pnlStatic")).style.display = "block";
					bn_gContainerPreifx = "RA";
		 // Show RSI
		}		 
} else {
		//setting test cookie
		if (bn_location_href.search(/^https?:\/\/[^\/]*cdwg.com/)> -1) {
				bn_setCookie('bn_cdw_ab','true','cdwg.com',90);
			} else if (bn_location_href.search(/^https?:\/\/[^\/]*cdw.ca/)> -1) {
				bn_setCookie('bn_cdw_ab','true','cdw.ca',90);
			} else {
				bn_setCookie('bn_cdw_ab','true','cdw.com',90);
		if(document.getElementById("BN_pnlStatic")) {
		      (document.getElementById("BN_pnlStatic")).style.display = "block";
					bn_gContainerPreifx = "BN";
			} // Show Baynote		
		}
	}	
}
//function to get the purchase details
function baynote_getOrderInfo() {
	if (typeof (bnOrderId)!= 'undefined' && bnOrderId && typeof (bnOrderTotal)!= 'undefined' && bnOrderTotal) {
		baynote_tag.attrs.purchaseId = bnOrderId;
		baynote_tag.attrs.totalPurchases = bnOrderTotal;
		baynote_tag.attrs.purchaseDetails = bnOrderDetails;
		baynote_tag.specialTarget = true;
	}
}

//var bn_onClickHandler = function(clickedElement, exitInfo), created custom exit information for tracking directed revenue
function bn_onClickHandler(clickedElement, exitInfo) {
		var bn_prodPrice = "";
		var bn_prodUrl = "";
		var bn_prodId = "";
		var bn_prodQty = "";
		var exitResult = false;
		var bn_cartPageRegex = /https?:\/\/www.cdw\.com\/shop\/products\/default.aspx?.*?(\?|&)(edc|EDC)=([^&.]+.*)/g;
			
		if(typeof(bnObserver) != 'undefined' && typeof(bnObserver.defaultExitConfirmation) != 'undefined') {
			exitResult = bnObserver.defaultExitConfirmation(clickedElement,exitInfo);
		}
		if(clickedElement) {
			//checks if the click is in the recommendation and on the addToCart button and then creates addToCart Event
			if (bn_isNotEmpty(clickedElement.tagName) && clickedElement.tagName == "INPUT" && clickedElement.className == "addToCart") {
						bn_prodQty = clickedElement.previousSibling.previousSibling.value;
						if (bn_prodQty == ""){
							bn_prodQty = 1;
						} 
					
					var bn_addToCartBtnId = "";
					var bn_cartSqncNumber = "";
					var bn_regex01 = /([\d]+)/g;
					var bn_priceLbl = "";
					var bn_targetId = "";
					
					bn_addToCartBtnId = clickedElement.id;
					bn_cartSqncNumber = bn_addToCartBtnId.match(bn_regex01);			
					bn_priceLbl = bn_gContainerPreifx + "_ra_dlAccessories_ctl" + bn_cartSqncNumber + "_mpdAccessories_pc_lblAP";
					bn_targetId = bn_gContainerPreifx + "_ra_dlAccessories_ctl" + bn_cartSqncNumber + "_mpdAccessories_hlName";
					bn_prodPrice = document.getElementById(bn_priceLbl).innerHTML
					bn_prodPrice = bn_prodPrice.replace(/\$/,"");
					bn_prodPrice = bn_prodPrice.replace(/\,/,"");
					
					var bn_prodUrlLbl = bn_gContainerPreifx + "_ra_dlAccessories_ctl" + bn_cartSqncNumber + "_mpdAccessories_hlImageProduct";
					bn_prodUrl = document.getElementById(bn_prodUrlLbl).href;
					bn_prodId = bn_getUrlParam("EDC",bn_prodUrl);			
					if (bn_isNotEmpty(bn_prodUrl) && bn_isNotEmpty(bn_prodId) &&bn_isNotEmpty(bn_prodPrice) && bn_isNotEmpty(bn_prodQty)) {
						if(exitInfo != null && typeof(exitInfo.attrs) == "undefined") {
							exitInfo.attrs = new Object();
						}
							var bnOrderDetails = new Array();
							bnOrderDetails[0] = bn_prodId + ":" + bn_prodQty + ":" + bn_prodPrice;
							/*exitInfo.attrs.purchaseId = 'addToCart';
							exitInfo.attrs.totalPurchases = (prodQty*prodPrice);*/
							if (bn_targetId != "") {
						     		var gt = document.getElementById(bn_targetId).getAttribute("baynote_guide");
						    		if (typeof(gt) != "undefined"  &&  gt) exitInfo.baynote_guide = gt;
								
						    		var gr = document.getElementById(bn_targetId).getAttribute("baynote_req");
						     		if (typeof(gr) != "undefined"  &&  gr) exitInfo.baynote_req = gr;

						     		var bn = document.getElementById(bn_targetId).getAttribute("baynote_bnrank");
						     		if (typeof(bn) != "undefined"  &&  bn) exitInfo.baynote_bnrank = bn;

						     		var ir = document.getElementById(bn_targetId).getAttribute("baynote_irrank");
						     		if (typeof(ir) != "undefined"  &&  ir) exitInfo.baynote_irrank = ir;
							 }
							 
							exitInfo.attrs.prodId = bn_prodId;
							exitInfo.attrs.purchaseDetails = bnOrderDetails;				
							exitInfo.dest = 'http://www.cdw.com/shop/cart/default.aspx';
							exitInfo.baynote_guide_target = bn_prodUrl;
							exitInfo.attrs.target = bn_prodUrl;
							exitInfo.attrs.action = "AddToCart";
							exitResult = true;
						} else {
							return exitResult;
						}
					//checks if the click is on the page and on the addToCart button not in a recommendation and then creates addToCart Event
					} else if (bn_isNotEmpty(clickedElement.tagName) && clickedElement.tagName == "INPUT" && clickedElement.id == "_primaryProductInformation__ibtnAddToCart") {
					var bn_prodQtyHolder = document.getElementById("_primaryProductInformation__txtQty");			
					bn_prodQty = bn_prodQtyHolder.value;
						if (bn_prodQty == ""){
							bn_prodQty = 1;
						}
			 			if (document.getElementById('_primaryProductInformation__warranty__txtQuantity')){
						var bn_prodWarrantyQty = document.getElementById('_primaryProductInformation__warranty__txtQuantity').value;
						var bn_prodWarrantyAmt = document.getElementById('_primaryProductInformation__warranty__lblPrice').innerHTML;
						bn_prodWarrantyAmt = bn_prodWarrantyAmt.replace(/\$/,"");
						bn_prodWarrantyAmt = bn_prodWarrantyAmt.replace(/\,/,"");
						}							
						var bn_prodPriceHolder = document.getElementById('_primaryProductInformation__price_lblAP').innerHTML;
						bn_prodPrice = bn_prodPriceHolder;
						bn_prodPrice = bn_prodPrice.replace(/\$/,"");
						bn_prodPrice = bn_prodPrice.replace(/\,/,"");
						bn_prodUrl = window.location.href;
						bn_prodId = bn_getUrlParam("EDC",bn_prodUrl);			
						if (bn_isNotEmpty(bn_prodUrl) && bn_isNotEmpty(bn_prodId) &&bn_isNotEmpty(bn_prodPrice) && bn_isNotEmpty(bn_prodQty)) {
							if(exitInfo != null && typeof(exitInfo.attrs) == "undefined") {
								exitInfo.attrs = new Object();
							}
							var bnOrderDetails = new Array();
							bnOrderDetails[0] = bn_prodId + ":" + bn_prodQty + ":" + bn_prodPrice;
							if (bn_isNotEmpty(bn_prodWarrantyQty) && bn_isNotEmpty(bn_prodWarrantyAmt) && bn_prodWarrantyQty >= 1){
							bnOrderDetails[1] = "w-" +bn_prodId + ":" + bn_prodWarrantyQty + ":" + bn_prodWarrantyAmt;
							}
							/*exitInfo.attrs.purchaseId = 'addToCart';
							exitInfo.attrs.totalPurchases = (prodQty*prodPrice);*/
							exitInfo.attrs.prodId = bn_prodId;						
							exitInfo.attrs.purchaseDetails = bnOrderDetails;				
							exitInfo.dest = 'http://www.cdw.com/shop/cart/default.aspx';
							exitInfo.attrs.target = bn_prodUrl;
							exitInfo.attrs.action = "AddToCart";
							exitResult = true;
						} else {
							return exitResult;
							}
				//checking if the click was on a product detail link in the recommendation and then tracks prodId
				} else if (bn_isNotEmpty(clickedElement.tagName) && clickedElement.tagName == "A" && clickedElement.className == "productName") {					
						bn_clickId = clickedElement.id;
						bn_prodUrl = document.getElementById(bn_clickId).href;
						bn_prodId = bn_getUrlParam("EDC",bn_prodUrl);	
							
				if (bn_isNotEmpty(bn_prodUrl) && bn_isNotEmpty(bn_prodId)) {
					if(exitInfo != null && typeof(exitInfo.attrs) == "undefined") {
						exitInfo.attrs = new Object();
						}
							exitInfo.baynote_guide = "products";
							exitInfo.baynote_req = "products";
							exitInfo.baynote_bnrank = "1";	
							exitInfo.attrs.prodId = bn_prodId;		
							exitInfo.dest = bn_prodUrl;
							exitResult = true;
						} else {
							return exitResult;
							}
					//checking if the click was on a product detail image in the recommendation and then tracks prodId		
					} else if (bn_isNotEmpty(clickedElement.tagName) && clickedElement.tagName == "IMG" && clickedElement.parentNode.tagName == "A" && clickedElement.parentNode.className == "hlImageProduct") {
						bn_clickId = clickedElement.parentNode.id;
						bn_prodUrl = document.getElementById(bn_clickId).href;
						bn_prodId = bn_getUrlParam("EDC",bn_prodUrl);
						
								if (bn_isNotEmpty(bn_prodUrl) && bn_isNotEmpty(bn_prodId)) {
									if(exitInfo != null && typeof(exitInfo.attrs) == "undefined") {
										exitInfo.attrs = new Object();
									}	
									exitInfo.attrs.prodId = bn_prodId;		
									exitInfo.dest = bn_prodUrl;
									exitResult = true;
									} else {
										return exitResult;
									} 
						//add to cart recommendation tracking
						} else if (bn_isNotEmpty(clickedElement.tagName) && clickedElement.tagName == "IMG" && clickedElement.parentNode.tagName == "A" && clickedElement.title == "Add to Cart") {
									bn_prodPrice = clickedElement.parentNode.parentNode.previousSibling.innerHTML;
									bn_prodPrice = bn_prodPrice.replace(/\$/,"");
									bn_prodPrice = bn_prodPrice.replace(/\,/,"");
									bn_prodQty = "1";
									bn_prodUrl = clickedElement.parentNode.href;
									bn_prodId = bn_getUrlParam("EDC",bn_prodUrl);
								if (bn_isNotEmpty(bn_prodUrl) && bn_isNotEmpty(bn_prodId) && bn_isNotEmpty(bn_prodPrice) && bn_isNotEmpty(bn_prodQty)) {
									if(exitInfo != null && typeof(exitInfo.attrs) == "undefined") {
										exitInfo.attrs = new Object();
									}
									
									var bnOrderDetails = new Array();
									bnOrderDetails[0] = bn_prodId + ":" + bn_prodQty + ":" + bn_prodPrice;
									exitInfo.baynote_guide = "products";
									exitInfo.baynote_req = "products";
									exitInfo.baynote_bnrank = "1";							
									exitInfo.attrs.prodId = bn_prodId;						
									exitInfo.attrs.purchaseDetails = bnOrderDetails;
									exitInfo.baynote_guide_target = bn_prodUrl;									
									exitInfo.dest = 'http://www.cdw.com/shop/cart/default.aspx';
									exitInfo.attrs.target = bn_prodUrl;
									exitInfo.attrs.action = "AddToCart";									
									} else {
										return exitResult;
									}
						//checking if the clicked image link is from a recommendation on the addtocart page
						} else if (bn_isNotEmpty(clickedElement.tagName) && clickedElement.tagName == "IMG" && clickedElement.parentNode.className == "menulink" && clickedElement.parentNode.href.match(bn_cartPageRegex)){
									bn_prodUrl = clickedElement.parentNode.href;
									bn_prodId = bn_getUrlParam("EDC",bn_prodUrl);
																		
									if (bn_isNotEmpty(bn_prodUrl) && bn_isNotEmpty(bn_prodId)) {
									if(exitInfo != null && typeof(exitInfo.attrs) == "undefined") {
										exitInfo.attrs = new Object();
									}
									exitInfo.baynote_guide = "products";
									exitInfo.baynote_req = "products";
									exitInfo.baynote_bnrank = "1";	
									exitInfo.attrs.prodId = bn_prodId;		
									exitInfo.dest = bn_prodUrl;
									exitResult = true;								
									} else {
										return exitResult;
									}
						//checking if the clicked link is from a recommendation on the addtocart page
						} else if (bn_isNotEmpty(clickedElement.tagName) && clickedElement.tagName == "A" && clickedElement.className == "menulink" && clickedElement.href.match(bn_cartPageRegex)){
									bn_prodUrl = clickedElement.href;
									bn_prodId = bn_getUrlParam("EDC",bn_prodUrl);								
									if (bn_isNotEmpty(bn_prodUrl) && bn_isNotEmpty(bn_prodId)) {
									if(exitInfo != null && typeof(exitInfo.attrs) == "undefined") {
										exitInfo.attrs = new Object();
									}
									exitInfo.baynote_guide = "products";
									exitInfo.baynote_req = "products";
									exitInfo.baynote_bnrank = "1";	
									exitInfo.attrs.prodId = bn_prodId;		
									exitInfo.dest = bn_prodUrl;
									exitResult = true;								
									} else {
										return exitResult;
									}
						}
				}
		return exitResult;
	}
function bn_isNotEmpty(checkVar) {
	return (typeof(checkVar) != "undefined") && (checkVar != null) && (checkVar != "");
}

function bn_getUrlParam(name, url) {
	var regexS = "[\\?&]"+name+"=([^&#]*)";
	var regex = new RegExp(regexS,"i");
	var results = regex.exec(url);
	if(results == null)
		return "";
	else
		return unescape(results[1]);
}

function bn_setCookie(c_name,c_value,c_domain,expiredays) {
	var exdate=new Date();
	exdate.setDate(exdate.getDate()+expiredays);
	if (c_domain != null && c_domain != "") {
		document.cookie = c_name+"="+escape(c_value)+";domain="+c_domain+";path=/"+((expiredays==null)?"":";expires="+exdate.toGMTString());
	} else {
	document.cookie = c_name+"="+escape(c_value)+";path=/"+((expiredays==null)?"":";expires="+exdate.toGMTString());
	}
}

if(typeof(baynote_tag)!="undefined") {
	bn_showObserver();
	bn_waitForPolicy();
}