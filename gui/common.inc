<!--#INCLUDE VIRTUAL="/global/subs/errorhandling.inc"-->
<!--#INCLUDE VIRTUAL="/global/subs/MenuSubs.inc"-->
<!--#INCLUDE VIRTUAL="/global/subs/WebContentSubs.inc"-->
<!--#INCLUDE VIRTUAL="/webcontent/includes/WeightedRandomInclude.inc"-->
<!--#INCLUDE VIRTUAL="/webcontent/includes/WebSitePhoneNumbers.inc"-->
<!--#INCLUDE VIRTUAL="/global/subs/SiteTrackingSubs.inc"-->
<!--#INCLUDE VIRTUAL="/global/subs/CustomPageSubs.inc"-->
<%
'********************************************************************************************************************'
'*****														*****'
'*****										VERSION HISTORY:  COMMON.INC	*****'
'*****  Modified on:	April 3, 2006										*****'
'*****	Modified by:	Paul Haan										*****'
'*****	Changes:	Updated to include the Vendor MicroSite Header.						*****'
'*****														*****'
'*****  Modified on:	August 28, 2006										*****'
'*****	Modified by:	Paul Haan										*****'
'*****	Changes:	Updated to allow for Sidebar content files on all sites					*****'
'*****			- The old Variable MWLeftSideBarInclude was not renamed.				*****'
'*****			Updated the URLS for the homepage.							*****'
'*****			Fixed an issue with a font.								*****'
'*****														*****'
'*****  Modified on:	November 21, 2007									*****'
'*****	Modified by:	Baskaran Ganesan									*****'
'*****	Changes:	Updated to include [Print Friendly] for printable pages (lines:	374 - 376)Defect 10915  *****'
'*****	Impacted Sub:	writePageLayoutTable									*****'
'*****														*****'
'*****  Modified on:	February 22, 2008									*****'
'*****	Modified by:	Paul Haan										*****'
'*****	Changes:	Added the <a name=Content></a> anchor to the main content block for use with the	*****'
'*****			Skip Navigation Short Cut (and AccessKey) in the header					*****'
'*****	Impacted Sub:	writePageLayoutTable (line 482 as of this version)					*****'																*****'
'*****														*****'
'*****  Modified on:	June 23, 2008										*****'
'*****	Modified by:	Amit Mittal										*****'
'*****	Changes:	Changed the location of CoreMetrics tags from top of the page to the bottom.		*****'
'*****			Created a method 'WriteCoreMetricsStuff' to render the CoreMetrics pieces		*****'
'*****	Impacted Subs:	WriteCoreMetricsStuff, writePageLayoutTable						*****'																*****'
'*****
'*****		
'*****  Modified on:     August 18 2008																			*****'
'*****  Modified by:     Rengarajan Srinivasan
'*****  Changes:		 PRJ50313 - UrlRedirecting. Added condition to check if the url contains                *****'	
'*****                   /shop/products/faxable.asp. If so, redirect from old product page                     *****'    
'*****                   new product page [/shop/products/default.aspx] 												*****'
'*****		
'*****  Modified on:     May 29 2012																			*****'
'*****  Modified by:     Andre coetzee
'*****  Changes:		 PRJ52646 - Removed call to DrawTabTrackingTag function which invoked TabStatus.aspx    *****'	
'*****		
'*****  Modified on:     June 12 2012																			*****'
'*****  Modified by:     Andre Coetzee
'*****  Changes:		 PRJ52646 - Modified IncludeUrl to use Version 6 of Msxml2.ServerXMLHTTP2  *****'	
'********************************************************************************************************************'
' global variables
	dim glpageTitle
	dim glpageDescription
	dim glpageImage
	dim glPrintable
	dim glDisplayPrintableVersionLink
	dim glShowheaderOnly
	dim glheaderText
	dim glShowTitleBar
	dim glShowSideBar
	dim glShowRightSideBar
	dim glShowResources
	dim glFlagShowMainHeader
	dim glFlagSideHeader
	dim glEnableTopSpacing
	dim glEnableLeftSpacing
	dim glSupressNavigation
	dim glCurrentSite
	dim glCurrentSiteName
	dim glCurrentURL
	dim glRedirectURL
	dim glPageSecure
	dim targettop
	dim glMainSiteName, glMainSite
	Dim RunTimeErrors, RunTimeSubErrors
	dim glHelpTopic
	Dim glLogonLinkExempt
	Dim glCustomPage
	Dim glClosedLoop
	Dim glTrueClosedLoop
	Dim glCustomPageTitle
	Dim glUseBodyOnLoadEvent	'MUST HAVE bodyLoad() javascript method on client
	Dim glLabelPath, HeaderPromo, FooterFile
	Dim MWLeftSideBarInclude				' this is NOW a misnomer:  the MW is no longer applicable. This is for all sites.
	Dim glCompanyShortName, glWebSiteLogo
	Dim glRightSideBarFileName
	Dim glVendorSessionUser	
	Dim IsCmPgIDEqualsPgTitle	
	IsCmPgIDEqualsPgTitle = true
	Dim glValidationFormat

'*********************************************************************************************************************
'	Current Session information for Vendor Micro Site Sessions
	Dim VendorSessionArray, VendorSiteID, VendorSiteKey, VendorSiteName, VendorSiteDescription
	Dim VendorSiteHomePage, VendorSiteContractID, VendorSiteCompanyID, VendorSiteDirectoryName
	Dim VendorSiteExtranetIncludeFile, VendorSitePhoneNumber, VendorSitePromoSpots, VendorSiteRedirectID

	Dim VendorImageDirectory, VendorContentDirectory, VendorStyleSheet, VendorHeaderPromo
	Dim VendorResourceBarFile, VendorContentFile, VendorNavigationFile, VendorTopMenuFile, VendorFooterFile

	'Check the session master cookie for Vendor Session Value...
	If CBool(Len(Trim(GetCookieValue(glCookieVendorSession))) > 0 ) Then
		glVendorSessionUser = true
		VendorSiteKey = GetCookieValue(glCookieVendorSession)

		VendorSessionArray 				= WebSite_GetVendorMicroSiteByKey( VendorSiteKey )
		'VendorSiteID					= VendorSessionArray(vmsSiteID)
		VendorSiteName					= VendorSessionArray(vmsVendorName)
		VendorSiteDescription			= VendorSessionArray(vmsSiteDescription)
		VendorSiteHomePage				= VendorSessionArray(vmsHomePageURL)
		VendorSiteContractID			= VendorSessionArray(vmsContractID)
		VendorSiteCompanyID				= VendorSessionArray(vmsCompanyID)
		VendorSiteDirectoryName			= VendorSessionArray(vmsDirectoryName)
		VendorSiteExtranetIncludeFile	= VendorSessionArray(vmsExtranetIncludeFile)
		VendorSitePromoSpots			= VendorSessionArray(vmsPromoSpots)
		VendorSiteRedirectID			= VendorSessionArray(vmsRedirectID)
		VendorSitePhoneNumber			= VendorSessionArray(vmsPhoneNumber)

		'Set up the image and content directories to be used for the header.
		If GetThisSite() = www_cdw_com Then
			VendorImageDirectory	= "/cdw/vms/" & VendorSiteDirectoryName & "/"
			VendorContentDirectory	= "/promos/vms/cdw/" & VendorSiteDirectoryName & "/"
		ElseIf GetThisSite() = www_cdwg_com Then
			VendorImageDirectory	= "/cdwg/vms/" & VendorSiteDirectoryName & "/"
			VendorContentDirectory	= "/promos/vms/cdwg/" & VendorSiteDirectoryName & "/"
		ElseIf GetThisSite() = www_cdw_ca Then
			VendorImageDirectory	= "/canada/vms/" & VendorSiteDirectoryName & "/"
			VendorContentDirectory	= "/promos/vms/canada/" & VendorSiteDirectoryName & "/"
		End If

		' Set up the link for the style sheet.
		VendorStyleSheet		= "/webcontent" & VendorContentDirectory & "styles.css"

		'Get/Set the cache for the navigation/header files for the session...
		VendorContentFile		= GetContentFileFromCache("VendorSessionSidebarContent_" & VendorSiteKey, VendorContentDirectory & "ContentSB.html")
		VendorNavigationFile	= GetContentFileFromCache("VendorSessionNavigation_" & VendorSiteKey, VendorContentDirectory & "NavigationMenu.html")
		VendorResourceBarFile	= GetContentFileFromCache("VendorSessionResources_" & VendorSiteKey, VendorContentDirectory & "Resources.html")
		VendorTopMenuFile		= GetContentFileFromCache("VendorSessionTopMenu_" & VendorSiteKey, VendorContentDirectory & "TopMenu.html")
		VendorFooterFile		= GetContentFileFromCache("VendorSessionFooter_" & VendorSiteKey, VendorContentDirectory & "Footer.html")

		'If the site is set up for promo spots, add the call to the random include nugget.
		If VendorSitePromoSpots Then
			VendorHeaderPromo	= DrawWebContentRandomIncludeFile(VendorContentDirectory & "HeaderPromos/")
		Else
			VendorHeaderPromo	= "&nbsp;"
		End If
	Else
		glVendorSessionUser	= false
	End If

'*********************************************************************************************************************
'	Current Session information for Phone number and Contract Search
	Dim CurSessionKey, CurSessionContracts, CurContractID, CurSessionPhoneNumber, CurPhoneNumber, z

		CurSessionKey = GetSessionKey()
		'CurSessionContracts = GetSessionType(CurSessionKey, eaSessionContract)
		'CurSessionPhoneNumber = GetSessionType(CurSessionKey, eaSessionPhoneNumber)

		'If(IsArray(CurSessionPhoneNumber)) Then
			'CurPhoneNumber = CurSessionPhoneNumber(eaSessionValue,0)
		'End If
'*********************************************************************************************************************
'Body Event Constants
	const glBodyOnClick = 0
	const glBodyOnLoad = 1
	const glBodyOnUnload = 2
'*********************************************************************************************************************
'set defaults for global variables
	glPageSecure				= False
	glShowHeaderOnly 			= False
	glFlagShowMainHeader 		= True
	glCurrentSite				= GetThisSite()
	glCurrentSiteName			= GetSiteName(glCurrentSite)
	glCurrentURL 				= getThisPage()
	glRedirectURL 				= "/default.asp"
	glReferrerRequirements 		= glAnyReferrer
	glLogonRequirements 		= glAnyLogon
	glSecureRequirements 		= glAnySecure 'glRequireNonSecure
	glSupressNavigation			= false
	glDestination 				= null
	glPrintable					= False
	glCustomPage				= False
	glClosedLoop 				= False
	glTrueClosedLoop			= False
	glUseBodyOnLoadEvent		= False
	glCompanyShortName			= "CDW"
	glRightSideBarFileName		= null
	glDisplayPrintableVersionLink = False
	glValidationFormat = "global"

	' Catch run time error
	' glRequestData is defined in global.inc
'	If UCase(Request.ServerVariables("REQUEST_METHOD")) = "GET" Then
'		RunTimeErrors = Trim(Request.QueryString("err"))
'		RunTimeSubErrors = Trim(Request.QueryString("serr"))
'	ElseIf UCase(Request.ServerVariables("REQUEST_METHOD")) = "POST" Then
'		RunTimeErrors = Trim(Request.Form("err"))
'		RunTimeSubErrors = Trim(Request.Form("serr"))
'	End If

	RunTimeErrors = Trim(glRequestData("err"))
	RunTimeSubErrors = Trim(glRequestData("serr"))

	If Len(RunTimeErrors) > 0 OR Len(RunTimeSubErrors) > 0 Then
		glPageError = true
	End If

	If Len(RunTimeSubErrors) > 0 Then
		RunTimeSubErrors = Split(RunTimeSubErrors, ",")
	End If

	If request.querystring("printable") = "1" Then
		glPrintable 		= True
	End If

	If GetThisSite() = www_cdw_ca Then
		glCDWCanadaUser = true
	End If

	if glCDWCanadaUser = false AND GetThisSite() <> www_cdwg_com  Then
		If Len(Trim(GetCookieValue(eaCookieVerticalMarket))) > 0  Then
			If Trim(GetCookieValue(eaCookieVerticalMarket)) = eaVMMacWarehouse Then
				glMacWarehouseUser = True
			End If
		Else
			If InStr(Request.ServerVariables("HTTP_USER_AGENT"), "Mac") > 0 then
				' Set the portion of the cookie that doesn't depend on the querystring
				SwitchVerticalMarketCookie(eaVMMacWarehouse)
				glMacWarehouseUser = True
			End If
		End If
	ElseIf glCDWCanadaUser = false AND GetThisSite() = www_cdwg_com  Then
		If Len(Trim(GetCookieValue(eaCookieVerticalMarket))) > 0  Then
			If Trim(GetCookieValue(eaCookieVerticalMarket)) = eaVMCDWHealthCare Then
				glHealthcareUser = True
			End If
		End If
	End If
		MWLeftSideBarInclude	= null	'set to null initially so only MW will get the sb by default.
		glMainSiteName			= GetSiteName(www_cdw_com)
		glMainSite				= www_cdw_com
		glpageTitle 			= "CDW.com"
		glCoremetricsCategoryID = glMainSiteName

	If GetThisSite() = www_cdwg_com and glHealthcareUser Then
		glCompanyShortName		= "CDW Healthcare"
		glWebSiteLogo 			= "/global/header/logo_h.gif"
		glMainSiteName			= GetSiteName(www_cdwg_com)
		glMainSite				= www_cdwg_com
		glpageTitle 			= "CDW Healthcare"
		HeaderPromo				= DrawWebContentRandomIncludeFileWithWeight("/Includes/HeaderPromo/Healthcare/red/")
		FooterFile				= GetWebcontentIncludeFileString("/Includes/CDWG/footer.htm")
		glLabelPath				= "/cdw/gui/SBlabels/"
		'glHomepageURL			= "/webcontent/healthcare/default.asp"
	ElseIf GetThisSite() = www_cdwg_com Then
		glCompanyShortName		= "CDW&middot;G"
		glWebSiteLogo 			= "/global/header/logo_G.gif"
		glMainSiteName			= GetSiteName(www_cdwg_com)
		glMainSite				= www_cdwg_com
		glpageTitle 			= "CDWG.com"
		HeaderPromo				= DrawWebContentRandomIncludeFileWithWeight("/Includes/HeaderPromo/cdwg.com/red/")
		FooterFile				= GetWebcontentIncludeFileString("/Includes/CDWG/footer.htm")
		glLabelPath				= "/cdwg/gui/SBlabels/"
	ElseIf glMacWarehouseUser = True Then
		glLabelPath				= "/MacWarehouse/gui/SBlabels/"
		HeaderPromo				= DrawWebContentRandomIncludeFileWithWeight("/Includes/HeaderPromo/MacWarehouse/red/")
		MWLeftSideBarInclude	= GetWebcontentIncludeFileString("/Includes/MacWarehouse/globalSB.asp")
		glRightSideBarFileName	= "/Includes/MacWarehouse/Sidebars/default.html"
		FooterFile				= GetWebcontentIncludeFileString("/Includes/MacWarehouse/footer.htm")
		glWebSiteLogo 			= "/global/header/logo_MW.gif"
	ElseIf glCDWCanadaUser = True Then
		glCompanyShortName		= "CDW Canada"
		glLabelPath				= "/cdw/gui/SBlabels/"
		FooterFile				= GetWebcontentIncludeFileString("/Includes/Canada/footer.htm")
		HeaderPromo				= DrawWebContentRandomIncludeFile("/Includes/HeaderPromo/cdw.ca/red/")
		glWebSiteLogo			= "/global/header/logo_CA.gif"
		glMainSiteName			= GetSiteName(www_cdw_ca)
		glMainSite				= www_cdw_ca
		glpageTitle 			= "CDW.ca"
	Else
		glLabelPath				= "/cdw/gui/SBlabels/"
		HeaderPromo				= DrawWebContentRandomIncludeFileWithWeight("/Includes/HeaderPromo/cdw.com/red/")
		FooterFile				= GetWebcontentIncludeFileString("/Includes/CDW/footer.htm")
		glWebSiteLogo			= "/global/header/logo_C.gif"
	End If

	If CBool(Len(Trim(GetCookieValue(eaCookieCustomPage))) > 0 ) Then
		glCustomPage = True

		glCustomPageTitle = GetCustomPageName(GetCookieValue(eaCookieCustomPage))
		If IsNull(glCustomPageTitle) And IsEmpty(glCustomPageTitle) And Len(glCustomPageTitle) <> 0 Then
			glCustomPageTitle = "Custom Page"
		End If
	End If

	If CBool(Len(Trim(GetCookieValue(eaCookieClosedLoop))) > 0 ) Then
		glClosedLoop 		= True
		If Len(Trim(GetCookieValue(glCookieeProcurementKey))) > 0 or Len(Trim(GetCookieValue(glCookieTransactiveKey))) > 0 Then
			glTrueClosedLoop 		= True
		End If
	End If

	glLogonLinkExempt 		= CBool(InStr(LCase(glCurrentURL), "logon.asp") > 0)
	glpageDescription		= Null
	glFlagSideHeader		= False
	glShowTitleBar 			= False
	glShowSideBar 			= True
	glShowRightSideBar		= False
	glShowResources			= True
	glEnableTopSpacing		= True
	glEnableLeftSpacing		= True

	glHelpTopic				= Null
	'-------------------------------------------------------------------------
	' Web Reporting Cookies
	'	- TrackKey cookie... for the lifetime of the user
	'	- TrackSessionKey cookie... for this particular session
	If(Not(Len(Trim(GetCookieValue(eaCookieTrackKey))) > 0)) Then
		'Paul Haan (12-31-2004):  Removed unused if statements
		'Response.Cookies(eaCookieTrackKey).Domain = GetWebSiteCookieDomainName()
		'Response.Cookies(eaCookieTrackKey).Path = "/"
		'Response.Cookies(eaCookieTrackKey) = CreateGUID()
		'Response.Cookies(eaCookieTrackKey).Expires = DateAdd("yyyy", 1, Date()) ' Kill me in one year
        
        Call SetSingleCookie(eaCookieTrackKey, CreateGUID(), DateAdd("yyyy", 1, Date()), Empty )
	End If
	If(Not(Len(Trim(GetCookieValue(eaCookieTrackSessionKey))) > 0)) Then
		'Paul Haan (12-31-2004):  Removed unused if statements
		Call SetCookieValue (eaCookieTrackSessionKey, CreateGuid())
	End If
	'-------------------------------------------------------------------------

	'Set SessionKey cookie...
	If(Not(Len(Trim(GetCookieValue(eaCookieSessionKey))) > 0)) Then
		glSessionKey = CreateGUID()
		SetCookieValue eaCookieSessionKey, glSessionKey
	Else
		glSessionKey = Trim(GetCookieValue(eaCookieSessionKey))
	End If

	'See if they are a SpecialCustomPage
	'Variables prefixed with "Current" to avoid name collision in /shop/contracts/contractcart.asp
	Dim CurrentCustomPageID, CurrentCustomPage, CustomPageBanner, IsSpecialCustomPage

	IsSpecialCustomPage = False

	CurrentCustomPageID = GetCustomPageID(GetCookieValue(eaCookieCustomPage))
	CurrentCustomPage = GetCustomPage(CurrentCustomPageID)
	If IsArray(CurrentCustomPage) Then
		CustomPageBanner = CurrentCustomPage(cpBannerImage)
		If Not IsNull(CustomPageBanner) And Not IsEmpty(CustomPageBanner) Then
			If CustomPageBanner <> "" Then
				IsSpecialCustomPage = True
			End If
		End If
	End If

'*********************************************************************************************************************

	call writePageLayoutTable()

'	Writes out the top, sidebar, and toolbar for the main table.
	sub writePageLayoutTable()
		dim TSidebarLinksArray
		dim BSidebarLinksArray
		dim sidebarContentArray
		Dim RedirectSite

		' Get RedirectSite
		If(GetThisProperty() = coCdw) Then
			RedirectSite = GetSiteName(www_cdw_com)
		ElseIf(GetThisProperty() = coCdwg) Then
			RedirectSite = GetSiteName(www_cdwg_com)
		ElseIf(GetThisProperty() = coCdwca) Then
			RedirectSite = GetSiteName(www_cdw_ca)
		End If

		'start ASP Redirects PRJ50312 Defect 5480 Fix
		
		Dim xmlRedirectDOM
		Dim xmlRedirectSourcePath
		Dim xmlRedirectNode
		Dim xmlTargetNode

		set xmlRedirectDOM = CreateObject("Microsoft.XMLDOM")		
		xmlRedirectDOM.setProperty "SelectionLanguage", "XPath"
		xmlRedirectDOM.async = false
		xmlRedirectSourcePath= Server.MapPath("/redirects.xml")		
		xmlRedirectDOM.load(xmlRedirectSourcePath)
		
		If (len(Request.ServerVariables("QUERY_STRING")) > 0) Then
			Dim qStringValues
			qStringValues= URLEncode( Request.ServerVariables("QUERY_STRING") )			
			'PRJ50313 - Url Rewriting
			if(lcase(request.servervariables("PATH_INFO") = "/shop/products/faxable.asp") OR lcase(request.servervariables("PATH_INFO") = "/shop/tools/compare/results.asp")) then
				Set xmlRedirectNode = xmlRedirectDOM.selectNodes("//redirects/redirect[@sourceurl='" & request.servervariables("PATH_INFO") & "']")
			Else		
				Set xmlRedirectNode =  xmlRedirectDOM.selectNodes("//redirects/redirect[translate(@sourceurl,'ABCDEFGHIJKLMNOPQRSTUVWXYZ', 'abcdefghijklmnopqrstuvwxyz') = '" & lcase(request.servervariables("PATH_INFO") & "?" & qStringValues )& "']")
			End if		
			'END ASP Redirects PRJ50313
		Else
			Set xmlRedirectNode = xmlRedirectDOM.selectNodes("//redirects/redirect[@sourceurl='" & request.servervariables("PATH_INFO") & "']")
		End If

		If (xmlRedirectNode.length > 0) Then		
			Set xmlTargetNode = xmlRedirectNode.item(0).Attributes.item(1)	
			'PRJ50313 - Url Rewriting
			if(lcase(request.servervariables("PATH_INFO") = "/shop/products/faxable.asp")) Then
				If (InStr(lcase(Request.ServerVariables("QUERY_STRING")),"sps") > 0) Then
					Response.Redirect(xmlTargetNode.nodeValue & "?" & Request.ServerVariables("QUERY_STRING"))
				Else
					Response.Redirect(xmlTargetNode.nodeValue & "?" & Request.ServerVariables("QUERY_STRING")& "&sps=1")
				End If
			ElseIf(lcase(request.servervariables("PATH_INFO") = "/shop/tools/compare/results.asp")) Then
				IF(CountQuerystringValues() > 1) THEN
					Response.Redirect(xmlTargetNode.nodeValue & "?" & Request.ServerVariables("QUERY_STRING"))
					Response.End()
				End If
			Else
				Response.Redirect(xmlTargetNode.nodeValue)
			End if
			'END ASP Redirects PRJ50313
		End If
		'End ASP Redirects PRJ50312 Defect 5480 Fix

		' Auto logon...
		If(UserHasSavedCredentials()) Then
			If(Not(UserIsLoggedIn()) And LCase(Request.ServerVariables("URL")) <> "/shop/eaccount/logon/logon.aspx" ) Then
				If Not(InStr(GetThisPage(),"404.aspx") > 0) Then
					'Response.Redirect("http://" & RedirectSite & "/shop/eaccount/logontarget.asp?AutoLogOn=1&UserName=" & (Trim(GetCookieValue(eaCookieUserNameStore))) & "&UserPassword=" & UrlEncode(Trim(GetCookieValue(eaCookieUserPasswordStore))) & "&Site=" & UrlEncode(GetThisSite()) & "&target=" & UrlEncode(GetThisPage()))
					'Response.Redirect("http://" & RedirectSite & "/shop/eaccount/logon/logon.aspx")
					'Response.Write(LCase(Request.ServerVariables("URL"))) 
					'Response.End()
				End If
			End If
		End If

		' Get default category and page ID so page can override later in settings or constructor
		Dim cmArray
		cmArray = ParseCoremetricsUrl(glCurrentUrl)

		if(isarray(cmArray)) Then
			glCoremetricsCategoryID = cmArray(0)
			glCoremetricsPageID = cmArray(1)
		End If

		Call Fire_OnGetPageConstructor()
		Call Fire_OnGetPageSettings()

		'Updated to include [Print Friendly] for printable pages [Defect 10915]
		if glPrintable then
			glpageTitle = glPageTitle & " [Print Friendly]"
			glCoremetricsPageID = glCoremetricsPageID & " [Print Friendly]"
		End If

		If Not(InStr(GetThisPage(),"404.aspx") > 0) Then
			if lcase(glPageTitle) <> "cdw.com" AND lcase(glPageTitle) <> "cdw.ca" AND lcase(glPageTitle) <> "cdwg.com" Then
				IF IsCmPgIDEqualsPgTitle then
					glCoremetricsPageID = glPageTitle
				End IF				
			End If
		End If

		If glVendorSessionUser Then
			glCoremetricsPageID = glCoremetricsPageID & "  (" & VendorSiteName & " Session)"
		End If

		if IsNull(glMetaDataDescription) OR IsEmpty(glMetaDataDescription) OR Len(glMetaDataDescription) = 0 Then
			if lcase(glPageTitle) <> "cdw.com" AND lcase(glPageTitle) <> "cdw.ca" AND lcase(glPageTitle) <> "cdwg.com" Then
				glMetaDataDescription = glPageTitle
			End If
		End If

		Call ValidatePageContext(glCurrentURL, glRedirectURL)
		Call WriteHeadBodyTag()

		'******  Print out a warning if you are on either local, beta or staging...
'		If ( glDevelopmentMode = glLocalMode or _
'			glDevelopmentMode = glBetaMode or _
'			glDevelopmentMode = glQAMode or _
'			glDevelopmentMode = glStagingMode or _
'			glDevelopmentMode = glPreviewMode _
'		) Then
'			Response.Write(_
'				"<div class=environmentBanner>" &_
'					""  &_
'						"" )
'							select case glDevelopmentMode
'								case glLocalMode
'									Response.write("You are in LOCAL")
'								case glBetaMode
'									Response.write("You are in BETA")
'								case glQAMode
'									Response.write("You are in QA")
'								case glStagingMode
'									Response.write("You are in STAGING")
'								case glPreviewMode
'									Response.write(UCase("SITE PREVIEW:  preview.cdw.com"))
'								case else
'									Response.write("UNKNOWN MODE!!!")
'							end select	
'					Response.write(_
'						""  &_
'					"" &_
'				"</div>")
'		End If

		if glPrintable then 					' If page is printable, only show CDW Logo with white background
			Call WritePrintablePage()
		elseif glSupressNavigation Then
			Call WriteNavigationSupressedPage()
		End If

		'Added 10/24/2003 by Paul Haan
		'	This extra table is for the left justification of the site.
		'	It will make it as close to 800 wide unless the content of the page pushes it wider...
		'Response.Write(_
		'	"<table border='0' cellspacing='0' cellpadding='0'  width='100%' bordercolor='orange' id='jMain' summary='Main table for page: sidebar and content'>" & _
		'		"<tr>" &_
		'			"<td valign='top'>" )

	if glPrintable = false AND glSupressNavigation = false then
		if glShowHeaderOnly = True then		' This variable is for pages to be used in frames
			If glFlagShowMainHeader Then
				If IsSpecialCustomPage Then
					Call writeSpecialCustomPageSiteHeader()
				ElseIf Not(glVendorSessionUser) Then
					Call writeHeader(glheaderText)
				Else
					Call writeVendorMicroSiteHeader()
				End If
			Else
				'Call writePetiteHeader()
				IncludeUrl("http://" & Request.ServerVariables("SERVER_NAME") & "/Header.aspx?aspbootstrap=1&URL=" & UrlEncode(GetThisPage()) & "&DisplayPrintable=" & glDisplayPrintableVersionLink)
			End If
		else
			if glFlagShowMainHeader then
				If IsSpecialCustomPage Then
					'Call writeSpecialCustomPageSiteHeader()
					IncludeUrl("http://" & Request.ServerVariables("SERVER_NAME") & "/Header.aspx?aspbootstrap=1&URL=" & UrlEncode(GetThisPage()) & "&DisplayPrintable=" & glDisplayPrintableVersionLink)
				ElseIf Not(glVendorSessionUser) Then					
					'Call writeHeader(glheaderText)
					IncludeUrl("http://" & Request.ServerVariables("SERVER_NAME") & "/Header.aspx?aspbootstrap=1&URL=" & UrlEncode(GetThisPage()) & "&DisplayPrintable=" & glDisplayPrintableVersionLink)					
				Else			
					'Call writeVendorMicroSiteHeader()
					IncludeUrl("http://" & Request.ServerVariables("SERVER_NAME") & "/Header.aspx?aspbootstrap=1&URL=" & UrlEncode(GetThisPage()) & "&DisplayPrintable=" & glDisplayPrintableVersionLink)
				End If
			else
				If glFlagSideHeader = True Then
					Call writeSideHeader()
				Else		
					Call writePetiteHeader()		
				End If
			end if

			Response.write("<div class='box'><a name='Content'></a><p class='boxcontent'><table width='100%' border='0'><tr><td align='center'>")
			Call DrawPageErrors()		' Write the new error handling block...
			Call Fire_OnGetMainBodyContent()

				Response.write(_
				"</td></tr></table></p>" &_
				"</div>")
				'Call writeFooter
				'Response.Write("<script src='/Footer.aspx'></script>")
				If glFlagShowMainHeader = True Then
					IncludeUrl("http://" & Request.ServerVariables("SERVER_NAME") & "/Footer.aspx?aspbootstrap=1")
				End if

				' Have to put the floating divs at the bottom for IE5 on Mac OS 9 menus. >:(
				' Don't put this code in any tables, or wrap it with anything else. 
				' It must appear directly in front of the </body> tag...
					If GetThisSite() = www_cdwg_com Then
						MenuSubs_GenerateSubMenus MenuSubs_CdwgMenu
					ElseIf glMacWarehouseUser = True Then
						MenuSubs_GenerateSubMenus MenuSubs_CdwMacMenu
					ElseIf GetThisSite() = www_cdw_ca Then
						MenuSubs_GenerateSubMenus MenuSubs_CdwCanadaMenu
					Else
						MenuSubs_GenerateSubMenus MenuSubs_CdwMenu
					End If

				' A div to serve as an expansion for IE5 on Mac OS 9, so that mouse
				' events are routed properly for the menus.
				Response.Write("<div id='blankDiv'></div>")

				'End BodyContent
					'temporary setup only for E-comm Initiatives team to capture customer data
				
				Call WriteCoreMetricsStuff()
				
				Response.write(_
					"</div></body>" & vbCrlf & _
					"</html>" & vbCrlf)
			Call Fire_OnGetPageDestructor()
		end if
	end if
	end sub	

	'********************************************************
	'* Subroutine name:	WriteCoreMetricsStuff
	'* Description:		Writes the CoreMetrics related HTML
	'* Parameter(s):		None
	'*********************************************************
	Sub WriteCoreMetricsStuff()

		' Coremetrics javascript files
		If glSiteTrackingActive Then

			Call WriteCoremetricsJavascriptLibraries()

			Call Response.Write("<script language='javascript' type='text/javascript'>" & vbcrlf & _
								"<!--" & vbcrlf)

			' Render Coremetrics cmSetProduction tag *Must* follow the <body> tag
			Call WriteCoremetricsSetProductionTag()

			' Render Coremetrics PageView tag.  *Must* follow the <body> tag
			' If the page is not a 404 page, check if the page is set up for the techproperties tag.
			If Not(InStr(GetThisPage(), "404.aspx") > 0) Then

				If Not(glCoremetricsRenderTechProp) Then	'Not set up for Tech Properties Tag: Throw page view tag

					If glCoremetricsRenderPageView Then

						Call WriteCoremetricsPageViewTag()

						If glCoremetricsRenderRegistration Then
							Call Response.write(glCorementricsRegistrationTag)
						End If

						If glCoremetricsRenderOrderView Then
							Call Response.write(glCorementricsShopAction9Tags)
							Call Response.write("	cmDisplayShop9s();" & vbCrLf )
							Call Response.write(glCorementricsOrderViewTag)
						End If

					End If

				Else

					'Set up for Tech Properties Tag:  Intro scripts and tech properties tag...
					Call WriteCoremetricsTechPropertiesTag()

				End If

			Else

				'Write the error tag for 404 pages.
				Call WriteCoremetricsErrorTag()

			End If

			
			Call Response.write("//-->" & vbcrlf &_
								"</script>" & vbcrlf)
		
		End If
		
	End Sub				



'************************************************************************************************************************************
Sub WriteOutCSSLink()

'***************************************  HTTP_USER_AGENT Calls on Mac, Win & Linux Machines  ***************************************
'*																																	*
'*	HTTP_USER_AGENT calls from the iMac 																							*
'*		Mac_IE  ==  HTTP_USER_AGENT = Mozilla/4.0  (compatible; MSIE 4.5; Mac_PowerPC)												*
'*	 	Mac_NS 	==  HTTP_USER_AGENT = Mozilla/4.61 (Macintosh; I; PPC)																*
'*	HTTP_USER_AGENT calls from Paul's system																						*
'*		Win_IE  ==  HTTP_USER_AGENT = Mozilla/4.0 (compatible; MSIE 5.01; Windows NT)												*
'*	 	Win_NS 	==  HTTP_USER_AGENT = Mozilla/4.6 [en] (WinNT; U)																	*
'*		Win_TV  ==  HTTP_USER_AGENT = Mozilla/4.0 WebTV/2.5 (Compatible; MSIE 2.0)													*
'* 		Win_OP  == 	HTTP_USER_AGENT = Mozilla/4.0 (compatible; MSIE 5.0; Windows NT 5.0) Opera 5.01  [en]							*
'* 		Win_OP  == 	HTTP_USER_AGENT = Mozilla/4.0 (compatible; MSIE 5.0; Windows 2000) Opera 5.11  [en]								*
'*	HTTP_USER_AGENT calls from the Linux test system																				*
'*	 	Lin_NS 	==  HTTP_USER_AGENT = Mozilla/4.51 [en] (X11; I; Linux 2.2.5-15 i686)												*
'*		Lin_NS  ==	HTTP_USER_AGENT = Mozilla/5.0 [en] (X11; U; Linux 2.2.5-15 i686; en-US; m18) Gecko/20001107 Netscape6/6.0		*
'*	HTTP_USER_AGENT calls from Aarons G4 with Safari																				*
'*		Safari  == HTTP_USER_AGENT = Mozilla/5.0 (Macintosh; U; PPC Mac OS X; en-us) AppleWebKit/85 (KHTML, like Gecko) Safari/85	*
'*		Mac_FF  == HTTP_USER_AGENT = Mozilla/5.0 (Macintosh; U; PPC Mac OS X Mach-O; en-us; rv:1.7.5) Gecko/20041107  FireFox/1.0	*
'*																																	*
'************************************************************************************************************************************

	dim CSSFile
	CSSFile = "Mac_IE"

		If InStr(Request.ServerVariables("HTTP_USER_AGENT"), "Mac") > 0 then
			If InStr(Request.ServerVariables("HTTP_USER_AGENT"), "MSIE") > 0 then
				CSSFile = "Mac_IE"
			ElseIF InStr(lcase(Request.ServerVariables("HTTP_USER_AGENT")), "safari") > 0 Then
				CSSFile = "Safari"
			ElseIF (InStr(lcase(Request.ServerVariables("HTTP_USER_AGENT")), "firefox") > 0) OR (InStr(lcase(Request.ServerVariables("HTTP_USER_AGENT")), "camino") > 0) Then
				CSSFile = "Mac_FF"
			Else
				CSSFile = "Mac_NS"
			End If
		ElseIf InStr(Request.ServerVariables("HTTP_USER_AGENT"), "Linux") > 0 then
			CSSFile = "Lin_NS"
		ElseIf InStr(Request.ServerVariables("HTTP_USER_AGENT"), "Win") > 0 then
			If InStr(Request.ServerVariables("HTTP_USER_AGENT"), "MSIE") > 0 then
				CSSFile = "Win_IE"
			Else 
				CSSFile = "Win_NS"
			End If
		End If

	' Call the style sheet
		Response.write("<link href='/global/gui/css/" & CSSFile & ".css' rel='stylesheet' type='text/css'>" )
		Response.write("<link href='/global/gui/css/Main.css' rel='stylesheet' type='text/css'>"  )
		If glVendorSessionUser Then
			Response.write("<link href='" & VendorStyleSheet & "' rel='stylesheet' type='text/css'>"  )
		ElseIf IsSpecialCustomPage Then
			Response.write("<link href='" & GetImagePath("/cdw/custompages/styles.css") & "' rel='stylesheet' type='text/css'>"  )
		End If
		Response.write("<link href='/global/gui/css/global/links.css' rel='stylesheet' type='text/css'>"  )
		Response.write("<link href='/global/gui/css/global/text.css' rel='stylesheet' type='text/css'>"  )
		Response.write("<link href='/global/gui/css/Layouts/shared.css' rel='stylesheet' type='text/css'>"  )
		Response.write("<link href='/global/gui/css/Layouts/cdw.css' rel='stylesheet' type='text/css'>"  )
		
		
		'Response.write("<link href='/content/testing/assets/pages/framework/global.css' rel='stylesheet' type='text/css'>"  )
		Response.write("<link href='/assets/pages/framework/typeahead.css' rel='stylesheet' type='text/css'>"  )
		Response.write("<link href='/global/gui/styles/dist/pages/search.css' rel='stylesheet' type='text/css'>"  )
		Response.write("<link href='/global/gui/stylesheets/global.css' rel='stylesheet' type='text/css'>"  )
		Response.write("<link href='/assets/pages/framework/nav.css' rel='stylesheet' type='text/css'>"  )
		Response.write("<link href='/assets/pages/framework/text.css' rel='stylesheet' type='text/css'>"  )
		Response.write("<link href='/assets/pages/content/contentglobal.css' rel='stylesheet' type='text/css'>"  )
		Response.write("<link href='/assets/pages/content/contenttext.css' rel='stylesheet' type='text/css'>"  )         
end sub

'*********************************************************************************************************************
	'Writes out the autocomplete js file link.
Sub WriteAutoCompleteJSLink()
	Response.Write("<script language='javascript' type='text/javascript' src='/global/gui/javascript/JQuery/JSON/jquery-json-2.2.js'></script>"  )
	Response.Write("<script language='javascript' type='text/javascript' src='/global/gui/javascript/JQuery/UI/jquery.ui.base.js'></script>"  )
	Response.Write("<script language='javascript' type='text/javascript' src='/global/gui/javascript/JQuery/Cdw.Core/jquery.cdw.ui.typeahead.js'></script>"  )
	
end sub
'*********************************************************************************************************************
	'Writes out the header and title.
Sub WriteHeadBodyTag()
	dim mac

	Response.Write(_
		"<!DOCTYPE HTML PUBLIC '-//W3C//DTD HTML 4.0 Transitional//EN'>"  &_
		"<html lang='EN'>"  & _
		"<head>" & _
			"<META HTTP-EQUIV='CONTENT-TYPE' CONTENT='text/html; charset=ISO-8859-1'>" &_
			"<script language='javascript' type='text/javascript' src='/global/gui/javascript/glwindow.js'></script>" & _
			"<script language='javascript' type='text/javascript' src='/global/gui/javascript/JQuery/1.5.1/jquery-1.5.1.js'></script>" & _
			"<script language='javascript' type='text/javascript' src='/global/gui/javascript/JQuery/1.5.1/jquery-1.5.1.js'></script>" & _
			"<script language='javascript' type='text/javascript' src='/global/gui/javascript/global.js'></script>" )

			Response.write("<script language='javascript' type='text/javascript'>" )
		'Write out the client-side BODY event functions
		
		Call writeBodyEvents()

	Response.Write("</script>" )
		Call Fire_OnGetClientScripts()

		Response.write("<title>" & glpageTitle & "</title>")

		Call writeMetaData()		'writes META data if data is defined in page settings area.
		Call WriteOutCSSLink()
		Call WriteAutoCompleteJSLink()

Dim glMarginWidth, glMarginHeight

	glMarginWidth	= "5"
	glMarginHeight	= "5"

	if glUseBodyOnLoadEvent = True then
		'MUST HAVE bodyLoad() javascript method on client
		response.write(_
			"</head>"  &_
			"<body style='text-align:center'>")
	else
		response.write(_
			"</head>" &_
			"<body style='text-align:center'>" )
	end if
end sub
'*********************************************************************************************************************
'Writes out the page without any navigation.
sub WriteNavigationSupressedPage()
	Response.write("<table border='0' cellpaddding=0 cellspacing='0' width='90%' summary='Table for body of navigation free pages'>")

	If glEnableTopSpacing = true Then
		Response.write("<tr><td height='7'><img src='" & GetImagePath("/global/pixels/none.gif") & "' height='7' width='7'></td></tr>")
	End If

		Response.write("<tr>")

		If glEnableLeftSpacing = true Then
			Response.write("<td width='7'><img src='" & GetImagePath("/global/pixels/none.gif") & "' height='7' width='7'></td>")
		End If

	Response.write(_
				"<td valign='top' >")

					Call Fire_OnGetMainBodyContent()

	Response.write(_
				"</td>" &_
			"</tr>" &_
		"</table>"     &_
		"<!-- End BodyContent -->"   &_
	"</body></html>")
end sub
'*********************************************************************************************************************
'Writes out the printable header information.
sub writePrintablePage()
	Dim LogoName, AltText
	Dim IsBeta
	
	IsBeta = false
	
	If (Request.QueryString("IsBeta") = "1") Then
		IsBeta = true
	End If
	
	Response.write(_
		"<table width='85%' cellpadding='5' cellspacing='5' border='0' summary='table for printable page header'>" &_
			"<tr>" &_
				"<td align='left' rowspan='2'>")

				If GetThisSite = www_cdwg_com And glHealthcareUser Then
					LogoName	= "/global/logos/cdwg/cdwhealthcareprint.gif"
					AltText		= "CDW Healthcare Logo"
				ElseIf GetThisSite = www_cdwg_com Then
					LogoName	= "/global/logos/cdwg/CDWGprint.gif"
					AltText		= "CDWG Logo"
				ElseIf glMacWarehouseUser = True Then
					LogoName	= "/global/logos/macWareHouse/logo_macwarehouse.gif"
					AltText		= "CDW  |  MacWarehouse"
				ElseIf glCDWCanadaUser = True Then
					LogoName	= "/global/logos/Canada/printable.gif"
					AltText		= "CDW Canada"
				Else
					LogoName	= "/global/logos/cdw/cdw_printablepage_logo.gif"
					AltText		= "CDW Logo"
				End If

				'VMS Specific stuff...
				if glVendorSessionUser Then
					If GetThisSite = www_cdwg_com Then
						LogoName	= "/global/logos/cdwg/CDWGprint.gif"
						AltText		= "CDWG Logo"
					Else
						LogoName	= "/global/logos/cdw/cdw_printablepage_logo.gif"
						AltText		= "CDW Logo"
					End If
				End If

				Response.write("<img src='" & GetImagePath(HTMLEncode(LogoName)) & "' border='0' alt='" & HTMLEncode(AltText) & "' />")

	Response.write(_
				"</td>" & _
				"<td align='right' valign='middle' nowrap><b>" &_
					"<font size=3 face='verdana,helvetica,arial' color='#000000'>")

					'10/31/07 this quick logic is to temporarily display beta phone# in header
					If (IsBeta = true) Then
						Response.write("888.503.4239")
					Else
					if glVendorSessionUser Then
							Response.write(HTMLEncode(VendorSitePhoneNumber))
						Else
							If Len(CurPhoneNumber) > 0 Then
								Response.write(HTMLEncode(CurPhoneNumber))
							Else
								Response.write(OnGetPhoneNumber())
							End If
						End If
					End if
					

				Response.write(_
					"</font></b>" & _
				"</td>" &_
			"</tr>" &_
		"</table>" & _
		"<table border='0' cellpaddding=0 cellspacing='0' width='90%' class='stylePrintIE7' summary='Table for body of printable version'>" &_
			"<tr>" &_
				"<td valign='top' >")
					if glEnableTopSpacing then
						Response.write("<img src='" & GetImagePath("/global/pixels/none.gif") & "' height='15' width='1' alt='' /><br />")
					end if

					Call Fire_OnGetMainBodyContent()

	Response.write(_
				"</td>" &_
			"</tr>" &_
		"</table>" &_
		"<br />" &_
		"<!-- End BodyContent -->"   &_
	"</body></html>")
end sub
'*********************************************************************************************************************
	'Writes out the page title and description.
Sub WritePageTitleBar()
	Response.write(_
		"<table border='0' bordercolor='#ff00ff' cellpadding='0' cellspacing='0' bgcolor='#ffffff' width='100%' summary='Table for the title of page'>" &_
			"<tr>")

			if len(glpageImage) > 0 then
				response.write(_
					"<td>" &_
						"<img src='" & glpageImage & "' align='left' alt='" & glpageImage & "' />&nbsp;" &_
					"</td>")
			end if

			response.write(_
				"<td width='70%'>" &_
					"<span class='bodyHeadLargeGrey'>" & glheaderText & "</span><br />" &_
					"<!--span class='bodytext>" & glPageDescription & "</span-->" &_
				"</td>" &_
				"<td width='100%' align='right'>&nbsp;</td>")

			Response.write(_
			"</tr>" &_
			"<tr>" &_
				"<td width='100%' colspan='3'>" &_
					"<img src='" & GetImagePath("/global/pixels/none.gif") & "' height='7' width='1' alt='' />" &_
				"</td>" &_
			"</tr>" &_
			"<tr>" &_
				"<td width='100%' colspan='3'>" &_
					"<img src='" & GetImagePath("/global/pixels/none.gif") & "' height='7' width='1' alt='' />" &_
				"</td>" &_
			"</tr>" &_
		"</table>"  )
end sub
'*********************************************************************************************************************
	'Writes out the header and title.
	sub writeHeader(glheaderText)

	if glShowHeaderOnly Then
		targettop = "target='_top'"
	else
		targettop = ""
	end if
	dim bgcolor, topnavbg, BorderColor

	'4/6/2005 - Paul Haan:  Removed the MW check to change ribbon to blue.
	If (GetThisSite() = www_cdwg_com) And Not glHealthcareUser Then
		BorderColor = "blue"
	Else
		BorderColor = "orange"
	End If
	bgcolor		= "CC0000"
	topnavbg	= "FFFFFF"

'Set the page to closed loop for testing.
'	glTrueClosedLoop = true
'Set the page to MW for testing.
'	glMacWarehouseUser = true

			Response.write(_
				"<table width='100%' height='82' cellpadding='0' cellspacing='0' border='0' bgcolor='#" & bgcolor & "' bordercolor='blue' summary='Main header table'>"  &_
					"<tr>"  &_
						"<td rowspan='2' valign='top' nowrap height='82'>" &_
							"<table cellpadding='0' height='82' cellspacing='0' border='0' bgcolor='#" & bgcolor & "' bordercolor='#0000FF' summary='Table for header logos'>"  &_
								"<tr>" )
									'4/6/2005 - Paul Haan:  Removed the CDWG check to draw the blank promo spot.
									'If GetThisSite = www_cdwg_com Then		'CDWG header: Logo and blank promo
									'	Response.write(_
									'		"<td valign='top' align='left'>" &_
									'			"<a href='http://" & glMainSiteName & glHomepageURL & "' " & targettop & ">" &_
									'				"<img src='" & GetImagePath(glWebSiteLogo) & "' border='0' vspace='0' alt='" & glCompanyShortName & "' />" &_
									'			"</A>" &_
									'		"</td>" &_
									'		"<td valign='top' align='left' bgcolor='#" & bgcolor & "' rowspan='2'>" & _
									'			"<img src='" & GetImagePath("/cdw/gui/headerPromos/Blank_" & BorderColor & ".gif") & "' border='0' alt='' />" &_
									'		"</td>" )
									If glDevelopmentMode = glPreviewMode Then		'Preview header: Logo and Promo spot
										Response.write(_
											"<td valign='top' align='left'>" &_
												"<a href='http://" & glMainSiteName &  glHomepageURL & "' " & targettop & ">" &_
													"<img src='" & GetImagePath(glWebSiteLogo) & "' border='0' alt='CDW' />" &_
												"</A>" &_
											"</td>" &_
											"<td valign='top' align='left' rowspan='2'>" &_
												"<a href='http://" & glMainSiteName & "/r.aspx?n=33227' " & targettop & ">" &_
													"<img src='" & GetImagePath("/cdw/gui/topimagePreview.gif") & "' border='0' alt='Please give us your feedback' />" &_
												"</a>" &_
											"</td>" )
									ElseIf CBool(Len(Trim(GetCookieValue(eaCookieCustomPage))) > 0 ) OR glTrueClosedLoop Then
										If glClosedLoop or glTrueClosedLoop Then		'Custom Page (closed loop) header: Logo and Promo spot
											Response.write(_
											"<td valign='top' align='left'>" &_
												"<a href='http://" & glMainSiteName & "/shop/custompages/default.aspx?CustomPageKey=" & HTMLEncode(GetCookieValue(eaCookieCustomPage)) & "' " & targettop & ">" &_
													"<img src='" & GetImagePath(glWebSiteLogo) & "' border='0' alt='" & glCompanyShortName & "' />" &_
												"</A>" &_
											"</td>" &_
											"<td valign='top' align='left' rowspan='2'>" &_
												"<img src='" & GetImagePath("/cdw/gui/headerPromos/Blank_" & BorderColor & ".gif") & "' border='0' alt='' />" &_
											"</td>" )
										Else	'Custom Page header: Logo and Promo spot
											Response.write(_
											"<td valign='top' align='left'>" &_
												"<a href='http://" & glMainSiteName & glHomepageURL & "' " & targettop & ">" &_
													"<img src='" & GetImagePath(glWebSiteLogo) & "' border='0' alt='" & glCompanyShortName & "' />" &_
												"</A>" &_
											"</td>" &_
											"<td valign='top' align='left' rowspan='2'>" &_
												"<img src='" & GetImagePath("/cdw/gui/headerPromos/Blank_" & BorderColor & ".gif") & "' border='0' alt='' />" &_
											"</td>" )
										End If
									Else		'Regular header: Logo and Promo spot
										Response.write(_
										"<td valign='top' align='left'>" &_
											"<a href='http://" & glMainSiteName & glHomepageURL & "' " & targettop & ">" &_
												"<img src='" & GetImagePath(glWebSiteLogo) & "' border='0' alt='" & glCompanyShortName & "' />" &_
											"</A>" &_
										"</td>" &_
										"<td valign='top' align='left' rowspan='2'>" & HeaderPromo & "</td>" )
									End If

				'This section of code will draw the CDW / MacWarehouse tabs for CDW.com and CDW.com/MacWarehouse
				Response.write(_
					"</tr>" & _
					"<tr>" & _
						"<td valign='bottom' height='18' width='150' ")

							If (glCDWCanadaUser = True OR GetThisSite() = www_cdwg_com) Then
								'Add the style to the TD because the Tabs are not displayed...
								Response.write(" bgcolor='#CC0000'><img src='" & GetImagePath("/global/header/" & BorderColor & "/Left_Border.gif") & "'  border='0' alt='' />")
							Else
								Response.write(">")	' End the TD tag because no style is needed...
								Response.write("<img src='" & GetImagePath("/global/header/" & BorderColor & "/Left_Border.gif") & "'  border='0' alt='' />" )
								If(glClosedLoop = False) AND (glTrueClosedLoop = False) Then
									'This is the new section for selecting the tabs between CDW and MacWarehouse
									If glMacWarehouseUser = true Then
										Response.Write(_
											"<a href='/ChangeVM.asp?svmc=" & eaVMCDW & "&target=" & glHomepageURL & "' " & targettop & ">" & _
												"<img src='" & GetImagePath("/global/header/tab_cdw_inactive.gif") & "'  border='0' alt='CDW' width='50' height='18' />" & _
											"</a>" & _
											"<a href='/ChangeVM.asp?svmc=" & eaVMMacWarehouse & "' " & targettop & ">" & _
												"<img src='" & GetImagePath("/global/header/tab_mw_active.gif") & "' border='0' alt='MacWarehouse' width='96' height='18' />" & _
											"</a>")
									Else
										Response.write(_
											"<a href='/ChangeVM.asp?svmc=" & eaVMCDW & "&target=" & glHomepageURL & "' " & targettop & ">" & _
												"<img src='" & GetImagePath("/global/header/tab_cdw_active.gif") & "' border='0' alt='CDW' width='50' height='18' />" & _
											"</a>" & _
											"<a href='/ChangeVM.asp?svmc=" & eaVMMacWarehouse & "' " & targettop & ">" & _
												"<img src='" & GetImagePath("/global/header/tab_mw_inactive.gif") & "' border='0' alt='MacWarehouse' width='96' height='18' />" & _
											"</a>")
									End If
								End If
							End If

				Response.write(_
						"</td>" & _
					"</tr>")
				'End Tab section for CDW.com and CDW.com/MacWarehouse

	Response.write(_
			"</table>" &_
		"</td>"  &_
		"<td width='100%' align='right' height='18' valign='top'>" )

			'Paul Haan:  12-30-2004
			'Moved the Top Navigation to a new Sub to make the code structure easier to read
			Call WriteTopNavigationLinks(topnavbg)

	Response.write(_
		"</td>" &_
		"<td class='sst' bgcolor='#FFFFFF'>&nbsp;</td>" & _
	"</tr>" )

'Build querystring for contracts for closed loop sites, and hidden input variables
Dim contractQryStr
Dim contractHiddenVars

If glClosedLoop Then
	contractQryStr = "?"
	contractHiddenVars = ""

	'for loop to run through contract when site is in closed loop mode
	'Session contracts
	'CurSessionContracts = GetSessionType(glSessionKey, eaSessionContract)

	If(IsArray(CurSessionContracts)) Then
		For z = LBOUND(CurSessionContracts,2) to UBOUND(CurSessionContracts,2)
			CurContractID = CurSessionContracts(eaSessionValue,z)
			contractHiddenVars = contractHiddenVars & "<input type='hidden' name='con' value='" & HTMLEncode(Trim(CurContractID)) & "' />"
			contractQryStr = contractQryStr & "con=" & HTMLEncode(Trim(CurContractID)) & "&"
		Next
	End If
End If

Dim textclass, borderIMG
	textClass = "sstWhite"
	borderIMG = "/global/header/" & BorderColor & "/Top_Border.gif"

	Response.write(_
	"<tr>"  &_
		"<td width='100%' valign='top' align='right' height='64' bgcolor='#" & bgcolor & "'>"  &_
			"<img src='" & GetImagePath(borderIMG) & "' alt='' height='10' width='100%' /><br />" )

			If Not(glTrueClosedLoop) Then
				'Paul Haan:  12-30-2004
				'Moved the search form to a new Sub to make the code structure easier to read
				Call WriteSearchArea(textClass, contractHiddenVars, contractQryStr)
			End If

	Response.write(_
		"</td>" &_
		"<td valign='top' height='64' width='10' align='right'>"  & _
			"<img src='" & GetImagePath("/global/header/" & BorderColor & "/Right_Border.gif") & "' width='10' border='0' alt=''>" & _
		"</td>" & _
	"</tr>"  &_
"</table>"  )

		if Not(glTrueClosedLoop) Then
			Call WriteCategoryMenu()
		Else
			Response.write(_
				"<table border='0' cellpadding='0' cellspacing='0' width='100%'>" & _
					"<tr>" & _
						"<td width='1' valign='top' align='right' bgcolor='#000000'>" & _
							"<img src='" & GetImagePath("/global/pixels/none.gif") & "' width='1' height='26' />" & _
						"</td>" &_
						"<td width='100%' nowrap valign='middle' align='right' class='num' style='font-weight:800; ")

							If glMacWarehouseUser = true Then
								Response.write("color:#666666;' bgcolor='#AAAAAA' background='" &  GetImagePath("/global/header/navBar/MW_Span.gif") & "'")
							Else
								Response.write("' background='" & GetImagePath("/global/header/navBar/C_Span.gif") & "' bgcolor='#AAAAAA' ")
							End If

			Response.write(_
						">" )

							If Len(CurPhoneNumber) > 0 Then
								Response.write(HTMLEncode(CurPhoneNumber))
							Else
								Response.write(OnGetPhoneNumber())
							End If
			Response.write(_
						"&nbsp;&nbsp;</td>" & _
						"<td width='1' valign='top' align='right' bgcolor='#000000'>" & _
							"<img src='" & GetImagePath("/global/pixels/none.gif") & "' width='1' height='26' />" & _
						"</td>" &_
					"</tr>"   &_
				"</table>")
	End if

end sub
'*********************************************************************************************************************
'This will write the small header if the main header is not needed.
sub writePetiteHeader()
	'Start Petite Header Code
	Dim bgcolor, Logopath, AltText, BorderColor, HexBorderColor
		bgcolor		= "#CC0000"
		

	'4/6/2005 - Paul Haan:  Removed the MW check to change ribbon to blue.
	If (GetThisSite() = www_cdwg_com) And Not glHealthcareUser Then
		BorderColor = "blue"
		HexBorderColor	="99CCFF"
	Else
		BorderColor = "orange"
		HexBorderColor	="FF9933"
	End If

	if glMacWarehouseUser = True Then
		Logopath	= "/global/header/petiteheader_C.gif"
	ElseIf GetThisSite = www_cdwg_com And glHealthcareUser Then
		Logopath	= "/global/header/petiteheader_h.gif"
	ElseIf GetThisSite = www_cdwg_com Then
		Logopath	= "/global/header/petiteheader_G.gif"
	ElseIf glCDWCanadaUser = True Then
		Logopath	= "/global/header/petiteheader_CA.gif"
	Else
		Logopath	= "/global/header/petiteheader_C.gif"
	End If

	if glVendorSessionUser Then
		If GetThisSite = www_cdwg_com Then
			Logopath	= "/global/header/petiteheader_G.gif"
		Else
			Logopath	= "/global/header/petiteheader_C.gif"
		End If
	End If
	Response.write(_
		"<table width='100%' cellpadding='0' cellspacing='0' border='0'   bordercolor='#FF0000' summary='Main header table'>" &_
			"<tr>" & _
				"<td nowrap>" & _
					"<img src='" & GetImagePath("/global/header/" & BorderColor & "/petiteheader_leftside.gif") & "' border='0' alt='' />" & _
				"</td>" & _
				"<td nowrap width='100%' bgcolor='" & bgcolor & "' style='border-top: 3px solid #" & HexBorderColor & ";'>")

					If glCustomPage <> true Then
						Response.write ( "<a href='http://" & glMainSiteName & glHomePageURL & "'>" )
					End If

					Response.write( "<img src='" & GetImagePath(Logopath) & "' border='0' alt='" & glCompanyShortName & "' />" )

					If glCustomPage <> true Then
						Response.write ( "</a>" )
					End If 
	Response.write(_
				"</td>" &_
				"<td width='10' valign='top' background='" & GetImagePath("/global/header/" & BorderColor & "/Right_Border.gif") & "'><img src='" & GetImagePath("/global/pixels/none.gif") & "' height='40' width='10' /></td>" & _
			"</tr>" &_
			"<tr>" & _
				"<td colspan='3' bgcolor='#" & HexBorderColor & "'><img src='" & GetImagePath("/global/pixels/none.gif") & "' height='3' width='1' /></td>" & _
			"</tr>" & _
		"</table>" )
	'End Petite Header Code
end sub

'*********************************************************************************************************************
'This will write the header for the special Custom Pages
sub writeSpecialCustomPageSiteHeader()
	'Build querystring for contracts for closed loop sites, and hidden input variables
	Dim contractQryStr
	Dim contractHiddenVars

	If glClosedLoop Then
		contractQryStr = "?"
		contractHiddenVars = ""

		'for loop to run through contract when site is in closed loop mode
		'Session contracts
		'CurSessionContracts = GetSessionType(glSessionKey, eaSessionContract)

		If(IsArray(CurSessionContracts)) Then
			For z = LBOUND(CurSessionContracts,2) to UBOUND(CurSessionContracts,2)
				CurContractID = CurSessionContracts(eaSessionValue,z)
				contractHiddenVars = contractHiddenVars & "<input type='hidden' name='con' value='" & HTMLEncode(Trim(CurContractID)) & "' />"
				contractQryStr = contractQryStr & "con=" & HTMLEncode(Trim(CurContractID)) & "&"
			Next
		End If
	End If

	If glCustomPage And glClosedLoop Then
		glHomepageURL = "/shop/custompages/Default.aspx?CustomPageKey=" & HTMLEncode(GetCookieValue(eaCookieCustomPage))
	End If

	Response.write(_
	"<table border='0' bordercolor='#CCCC00' cellpadding='0' cellspacing='0' width='100%' background='" & GetImagePath("/cdw/custompages/background.gif") & "'>" & _
		"<tr>" & _
			"<td class='cpCDWLogo'>" & _
				"<a href='" & glHomepageURL & "'><img src='" & GetImagePath("/cdw/custompages/cdwlogo.gif") & "' border='0' alt='" & glcompanyShortName & " Logo' /></a>" & _
			"</td>" & _
			"<td class='cpShowcase'>" & _
				"<a href='" & glHomepageURL & "'><img src='" & GetImagePath(CustomPageBanner) & "' border='0' alt='Banner' /></a>" & _
			"</td>" & _
			"<td class='cpHeaderPromos'>&nbsp;" &_
			"</td>" & _
			"<td class='cpToolsSearch'>" & _
				"<table cellpadding='0' cellspacing='0' border='0' bordercolor='#0000CC'>" & _
					"<tr>" & _
						"<td nowrap class='cpToolbar' colspan='3'>")

							Call WriteSpecialCustomPageTopNavigationLinks()

	Response.write(_
						"</td>" & _
					"</tr>" & _
					"<tr>" & _
						"<td rowspan='2' class='cpSearchSpacer'>&nbsp;" & _
							"<form name='globalSearch' id='globalSearch' action='http://" & glMainSiteName & "/shop/search/" & GetSearchPage() & "' " & targettop & " method='get'>" & _
						"</td>" & _
						"<td nowrap class='cpSearchBox'>" & _
							"<input type='text' id='key' NAME='key' class='searchbox' maxlength='75' />" & _
							"<input type='hidden' name='sr' value='1'>" &_
							contractHiddenVars &_
						"</td>" & _
						"<td nowrap class='cpSearchButton'>" & _
							"<INPUT type='image' alt='Search' src='" & GetImagePath("/global/buttons/gy_search.gif") & "' border='0' />" & _
						"</td>" & _
					"</tr>" & _
					"<tr>" & _
						"<td>" & _
							"<span class='cpSearchRadioButtons'>" & _
								"<table border='0' cellspacing='0' cellpadding='0' summary='Search filter table: Mac, PC, All'>" & _
									"<tr>" & _
										"<td align='right' class='sstWhite'>" & _
										"<input id='macsearch' name='platform' type='radio' value='mac' />" & _
										"<label for='macsearch'>Mac&nbsp;</label>" & _
										"<input id='pcsearch' type='radio' name='platform' value='pc' />" & _
										"<label for='pcsearch'>PC&nbsp;</label>" & _
										"<input id='allsearch' type='radio' name='platform' value='all' checked />" & _
										"<label for='allsearch'>All</label>" & _
										"</td>" & _
									"</tr>" & _
					  			"</table>" & _
							"</span>" & _
						"</td>" & _
					"</tr>" & _
				"</form></table>" & _
				"</td>" & _
				"<td align='right'><img src='" & GetImagePath("/cdw/custompages/rightEnd.gif") & "'></td>" & _
			"</tr>" & _
		"<tr>" & _
			"<td colspan='5'>")
			If Not(glTrueClosedLoop) Then
				Call WriteCategoryMenu()
			Else
				Response.write(_
					"<table border='0' cellpadding='0' cellspacing='0' width='100%'>" & _
						"<tr>" & _
							"<td width='1' valign='top' align='right' bgcolor='#000000'>" & _
								"<img src='" & GetImagePath("/global/pixels/none.gif") & "' width='1' height='26' />" & _
							"</td>" &_
							"<td width='100%' nowrap valign='middle' align='right' class='num' style='font-weight:800; ")

								If glMacWarehouseUser = true Then
									Response.write("color:#666666;' bgcolor='#AAAAAA' background='" &  GetImagePath("/global/header/navBar/MW_Span.gif") & "'")
								Else
									Response.write("' background='" & GetImagePath("/global/header/navBar/C_Span.gif") & "' bgcolor='#AAAAAA' ")
								End If

				Response.write(_
							">" )

								If Len(CurPhoneNumber) > 0 Then
									Response.write(HTMLEncode(CurPhoneNumber))
								Else
									Response.write(OnGetPhoneNumber())
								End If
				Response.write(_
							"&nbsp;&nbsp;</td>" & _
							"<td width='1' valign='top' align='right' bgcolor='#000000'>" & _
								"<img src='" & GetImagePath("/global/pixels/none.gif") & "' width='1' height='26' />" & _
							"</td>" &_
						"</tr>"   &_
					"</table>")
			End If
			Response.Write("</td>" & _
		"</tr>" & _
	"</table>")
		
end sub

'*********************************************************************************************************************
'This will write the header for the Vendor Microsites...
sub writeVendorMicroSiteHeader()

	If glVendorSessionUser Then
		If (UserIsLoggedIn) Then
			glHomepageURL = "/Default.asp"	'Set this so the redirect on the homepage will kick in...
		Else
			glHomepageURL = VendorSiteHomePage
		End If
	End If

	Response.write(_
	"<table border='0' bordercolor='#CCCC00' cellpadding='0' cellspacing='0' width='100%' background='" & GetImagePath(VendorImageDirectory & "background.gif") & "'>" & _
		"<tr>" & _
			"<td class='vmsCDWLogo'>" & _
				"<a href='" & glHomepageURL & "'><img src='" & GetImagePath(VendorImageDirectory & "cdwlogo.gif") & "' border='0' alt='" & glcompanyShortName & " Logo' /></a>" & _
			"</td>" & _
			"<td class='vmsShowcase'>" & _
				"<a href='" & glHomepageURL & "'><img src='" & GetImagePath(VendorImageDirectory & "showcase.gif") & "' border='0' alt='" & HTMLEncode(VendorSiteDescription) & "' /></a>" & _
			"</td>" & _
			"<td class='vmsHeaderPromos'>" &_
				 VendorHeaderPromo & _
			"</td>" & _
			"<td class='vmsToolsSearch'>" & _
				"<table cellpadding='0' cellspacing='0' border='0' bordercolor='#0000CC'>" & _
					"<tr>" & _
						"<td nowrap class='vmsToolbar' colspan='3'>")

							Call WriteVMSTopNavigationLinks()

	Response.write(_
						"</td>" & _
					"</tr>" & _
					"<tr>" & _
						"<td rowspan='2' class='vmsSearchSpacer'>&nbsp;" & _
							"<form name='globalSearch' id='globalSearch' action='http://" & glMainSiteName & "/shop/search/" & GetSearchPage() & "' " & targettop & " method='get'>" & _
						"</td>" & _
						"<td nowrap class='vmsSearchBox'>" & _
							"<input type='text' id='key' NAME='key' class='searchbox' maxlength='75' />" & _
							"<input type='hidden' name='sr' value='1'>" &_
						"</td>" & _
						"<td nowrap class='vmsSearchButton'>" & _
							"<INPUT type='image' alt='Search' src='" & GetImagePath(VendorImageDirectory & "search.gif") & "' border='0' />" & _
						"</td>" & _
					"</tr>" & _
					"<tr>" & _
						"<td>" & _
							"<span class='vmsSearchRadioButtons'>" & _
								"<table border='0' cellspacing='0' cellpadding='0' summary='Search filter table: Mac, PC, All'>" & _
									"<tr>" & _
						  				
										"<td align='right' class='sstWhite'>" & _
										"<input id='macsearch' name='platform' type='radio' value='mac' />" & _
										"<label for='macsearch'>Mac&nbsp;</label>" & _
										"<input id='pcsearch' type='radio' name='platform' value='pc' />" & _
										"<label for='pcsearch'>PC&nbsp;</label>" & _
										"<input id='allsearch' type='radio' name='platform' value='all' checked />" & _
										"<label for='allsearch'>All</label>" & _
										"</td>" & _

									"</tr>" & _
					  			"</table>" & _
							"</span>" & _
						"</td>" & _
					"</tr>" & _
				"</form></table>" & _
				"</td>" & _
				"<td align='right'><img src='" & GetImagePath(VendorImageDirectory & "rightEnd.gif") & "'></td>" & _
			"</tr>" & _
		"<tr>" & _
			"<td colspan='5'>" & VendorNavigationFile & "</td>" & _
		"</tr>" & _
	"</table>")
end sub

'*********************************************************************************************************************
'Writes out the top right navigation with the Home, About us and Log On/Off links.
Sub WriteVMSTopNavigationLinks()

	Dim LinkStyle
		LinkStyle = "class='vmsToolbarLink'"

		Response.write(_
			"<table border='0' bordercolor='#00FFFF' cellpadding='0' height='18' cellspacing='0' summary='Table for top customer links'>"  &_
				"<tr>" & _
				"<td height='18' nowrap valign='middle' align='left' width='100%'>&nbsp;</td>" & _
					"<td height='18' nowrap valign='middle' align='left' " & LinkStyle & ">|&nbsp;" & _
						"<a href='http://" & glMainSiteName & glHomepageURL & "' " & LinkStyle & " " & targettop & " title='Home'>Home</a>" & _
						"</td>" )

					Response.write(VendorTopMenuFile)

					Response.write(_
						"<td height='18' nowrap valign='middle' align='left' " & LinkStyle & ">&nbsp;|&nbsp;</td>"  &_
						"<td height='18' nowrap valign='middle' align='left'>" &_
							"<a href='http://" & glMainSiteName & glShoppingCartURL & "' " & LinkStyle & " " & targettop & " title='View Cart'>View Cart</a>" &_
						"</td>"  &_
						"<td height='18' nowrap valign='middle' align='left' " & LinkStyle & ">&nbsp;|&nbsp;</td>" )

					If (UserIsLoggedIn()) Then
						Response.Write(_
							"<td nowrap height='18' valign='middle' align='left' class='" & LinkStyle & "'>" &_
								"<a href='http://" & glMainSiteName & "/shop/eaccount/logoff.asp' " & LinkStyle & " " & targettop & " title='Log Off'>Log Off</a>&nbsp;" &_
							"</td>"  )
					Else
						Response.Write("<td nowrap height='18' valign='middle' align='left'>")

						If(Not(glLogonLinkExempt)) Then
							Response.Write("<a href='https://" & glMainSiteName & glBaseLogonURL & "?site=" & HTMLEncode(glCurrentSite) & "&target=" & HTMLEncode(glCurrentURL) &"' " & targettop & " class='vmsLogonLink'>")
						End If

						If glLogonLinkExempt Then
							Response.Write("<img src='" & GetImagePath(VendorImageDirectory & "login_disabled.gif") & "' border='0' alt='Logon not available on this page' />")
						Else
							Response.Write("<img src='" & GetImagePath(VendorImageDirectory & "login.gif") & "' border='0' alt='Logon' />")
						End If

						If(Not(glLogonLinkExempt)) Then 
							Response.Write("</a>") 
						End If

						Response.Write(_
							"</td>"  &_
							"<td nowrap height=18 valign='middle' align='left'>" &_
								"&nbsp;")

						If(Not(glLogonLinkExempt)) Then
							Response.Write("<a href='https://" & glMainSiteName & glBaseLogonURL & "?site=" & HTMLEncode(glCurrentSite) & "&target=" & HTMLEncode(glCurrentURL) & "' class='vmsLogonLink' " & targettop & " title='Log On'>")
						Else
							Response.Write("<span class='vmsLogonLinkExempt'>")
						End If

						Response.Write("Log On")

						If(Not(glLogonLinkExempt)) Then 
							Response.Write("</a>")
						Else
							Response.Write("</span>")
						End If
						Response.Write("</td>")
					end if
				Response.write(_
				"</tr>"  &_
			"</table>" )
End Sub
'*********************************************************************************************************************
'Writes out the top right navigation for special custompages
Sub WriteSpecialCustomPageTopNavigationLinks()
	dim AboutName, AboutPage, ServicePage
		ServicePage	= "/webcontent/inside/service/default.asp"
	If GetThisSite = www_cdwg_com Then
		AboutName	= "About Us"
		AboutPage	= "/r.aspx?n=59649"
		ServicePage	= "/webcontent/insideG/service/default.asp"
	ElseIf glCDWCanadaUser Then
		AboutName	= "About Us"
		AboutPage	= "/r.aspx?n=59650"
	Else
		AboutName	= "About Us"
		AboutPage	= "/r.aspx?n=59648"
	End If

	Dim LinkStyle
		LinkStyle = "class='cpToolbarLink'"

		Response.write(_
			"<table border='0' bordercolor='#00FFFF' cellpadding='0' height='18' cellspacing='0' summary='Table for top customer links'>"  &_
				"<tr>" & _
				"<td height='18' nowrap valign='middle' align='left' width='100%'>&nbsp;</td>" & _
					"<td height='18' nowrap valign='middle' align='left' " & LinkStyle & ">|&nbsp;" & _
						"<a href='http://" & glMainSiteName & glHomepageURL & "' " & LinkStyle & " " & targettop & " title='Home'>Home</a>&nbsp;|&nbsp;" & _
						"</td>" )

					Response.write(_
						"<td height='18' nowrap valign='middle' align='left' " & LinkStyle & "><a href='" & AboutPage & "' class='cpToolbarLink'  title='" & AboutName & "'>" & AboutName & "</a>&nbsp;|&nbsp;</td>" & _
						"<td height='18' nowrap valign='middle' align='left'>" &_
							"<a href='http://" & glMainSiteName & ServicePage & "' " & LinkStyle & " " & targettop & " title='Customer Support'>Customer Support</a>" &_
						"</td>" &_
						"<td height='18' nowrap valign='middle' align='left' " & LinkStyle & ">&nbsp;|&nbsp;</td>"  &_
						"<td height='18' nowrap valign='middle' align='left'>" &_
							"<a href='http://" & glMainSiteName & glShoppingCartURL & "' " & LinkStyle & " " & targettop & " title='View Cart'>View Cart</a>" &_
						"</td>"  &_
						"<td height='18' nowrap valign='middle' align='left' " & LinkStyle & ">&nbsp;|&nbsp;</td>" )

					If (UserIsLoggedIn()) Then
						Response.Write(_
							"<td nowrap height='18' valign='middle' align='left' class='" & LinkStyle & "'>" &_
								"<a href='http://" & glMainSiteName & "/shop/eaccount/logoff.asp' " & LinkStyle & " " & targettop & " title='Log Off'>Log Off</a>&nbsp;" &_
							"</td>"  )
					Else
						Response.Write("<td nowrap height='18' valign='middle' align='left'>")

						If(Not(glLogonLinkExempt)) Then
							Response.Write("<a href='https://" & glMainSiteName & glBaseLogonURL & "?site=" & HTMLEncode(glCurrentSite) & "&target=" & HTMLEncode(glCurrentURL) &"' " & targettop & " class='cpLogonLink'>")
						End If

						If glLogonLinkExempt Then
							Response.Write("<img src='" & GetImagePath("/cdw/custompages/login_disabled.gif") & "' border='0' alt='Logon not available on this page' />")
						Else
							Response.Write("<img src='" & GetImagePath("/cdw/custompages/login.gif") & "' border='0' alt='Logon' />")
						End If

						If(Not(glLogonLinkExempt)) Then 
							Response.Write("</a>") 
						End If

						Response.Write(_
							"</td>"  &_
							"<td nowrap height=18 valign='middle' align='left'>" &_
								"&nbsp;")

						If(Not(glLogonLinkExempt)) Then
							Response.Write("<a href='https://" & glMainSiteName & glBaseLogonURL & "?site=" & HTMLEncode(glCurrentSite) & "&target=" & HTMLEncode(glCurrentURL) & "' class='cpLogonLink' " & targettop & " title='Log On'>")
						Else
							Response.Write("<span class='cpLogonLinkExempt'>")
						End If

						Response.Write("Log On")

						If(Not(glLogonLinkExempt)) Then 
							Response.Write("</a>")
						Else
							Response.Write("</span>")
						End If
						Response.Write("</td>")
					end if
				Response.write(_
				"</tr>"  &_
			"</table>" )
End Sub
'*********************************************************************************************************************
'Writes out the top right navigation with the Home, About us and Log On/Off links.
Sub WriteTopNavigationLinks(topnavbg)
	Dim LinkStyle, BGColor
		LinkStyle = "class='MenuLink'"

		BGColor = "bgcolor='#" & topnavbg & "'"

		If Not(glTrueClosedLoop) Then
			Response.write(_
			"<table border='0' bordercolor='#00FFFF' cellpadding='0' height='18' cellspacing='0' " & BGColor & " width='100%' summary='Table for top customer links'>"  &_
				"<tr>" &_
					"<td height='18' width='100%' nowrap valign='middle' align='left' " & BGColor & ">" &_
						"<img src='" & GetImagePath("/global/pixels/none.gif") & "' height='18' width='100%' alt='' />" &_
					"</td>"&_
					"<td height='18' nowrap valign='middle' align='left' " & LinkStyle & ">|&nbsp;" )

						If Cbool(Len(Trim(GetCookieValue(eaCookieCustomPage))) > 0 ) Then
							If glClosedLoop Then
								Response.write("<a href='http://" & glMainSiteName & "/shop/custompages/default.aspx?CustomPageKey=" & HTMLEncode(GetCookieValue(eaCookieCustomPage)) & "' " & LinkStyle & " " & targettop & " title='Home'>Home</a>")
							Else
								Response.write("<a href='http://" & glMainSiteName & glHomepageURL & "' " & LinkStyle & " " & targettop & " title='Home'>Home</a>")
							End If
						Else
							Response.write("<a href='http://" & glMainSiteName & glHomepageURL & "' " & LinkStyle & " " & targettop & " title='Home'>Home</a>")
						End If
					Response.write(_
						"</td>" )

						dim AboutName, AboutPage, ServicePage
							ServicePage	= "/webcontent/inside/service/default.asp"
						If GetThisSite = www_cdwg_com Then
							AboutName	= "About Us"
							AboutPage	= "/r.aspx?n=59649"
							ServicePage	= "/webcontent/insideG/service/default.asp"
						ElseIf glCDWCanadaUser Then
							AboutName	= "About Us"
							AboutPage	= "/r.aspx?n=59650"
						Else
							AboutName	= "About Us"
							AboutPage	= "/r.aspx?n=59648"
						End If

					Response.write(_
					"<td height='18' nowrap valign='middle' align='left' " & LinkStyle & ">&nbsp;|&nbsp;</td>"  &_
					"<td height='18' nowrap valign='middle' align='left'>" &_
						"<a href='http://" & glMainSiteName & AboutPage & "' " & LinkStyle & " " & targettop & " title='" & AboutName & "'>" & AboutName & "</a>" &_
					"</td>" )

					If glMacWarehouseUser = false Then
					Response.write(_
						"<td height='18' nowrap valign='middle' align='left' " & LinkStyle & ">&nbsp;|&nbsp;</td>"  &_
						"<td height='18' nowrap valign='middle' align='left'>" &_
							"<a href='http://" & glMainSiteName & ServicePage & "' " & LinkStyle & " " & targettop & " title='Customer Support'>Customer Support</a>" &_
						"</td>" )
					End If

					Response.write(_
						"<td height='18' nowrap valign='middle' align='left' " & LinkStyle & ">&nbsp;|&nbsp;</td>" &_
						"<td height='18' nowrap valign='middle' align='left'>" &_
							"<a href='http://" & glMainSiteName & glShoppingCartURL & "' " & LinkStyle & " " & targettop & " title='View Cart'>View Cart</a>" &_
						"</td>" & _
						"<td height='18' nowrap valign='middle' align='left' " & LinkStyle & ">&nbsp;|&nbsp;</td>" )

						If glMacWarehouseUser then
							Response.write(_
							"<td height='18' nowrap valign='middle' align='left'>" &_
								"<a href='http://" & glMainSiteName & HtmlEncode(glCurrentURL) )
									If Len(request.querystring()) > 0 Then
										Response.write("&printable=1")
									Else
										Response.write("?printable=1")
									End If
							Response.write(_
								 "' target='new' " & LinkStyle & " title='Printable Page (popup window)'>Printable Page</a>" &_
							"</td>"  &_
							"<td height='18' nowrap valign='middle' align='left' " & LinkStyle & ">&nbsp;|&nbsp;</td>" )
						End If

						If(UserIsLoggedIn()) Then
							Response.Write(_
								"<td nowrap height='18' valign='middle' align='left' class='MenuLink'>" &_
									"<a href='http://" & glMainSiteName & "/shop/eaccount/logoff.asp' " & LinkStyle & " " & targettop & " title='Log Off'>Log Off</a>&nbsp;" &_
								"</td>"  )
						Else
							Response.Write("<td nowrap height='18' valign='middle' align='left'>")
	
							If(Not(glLogonLinkExempt)) Then
								Response.Write("<a href='https://" & glMainSiteName & glBaseLogonURL & "?site=" & HTMLEncode(glCurrentSite) & "&target=" & HTMLEncode(glCurrentURL) &"' " & LinkStyle & " style='FONT-WEIGHT:800;COLOR:#FF0000' " & targettop & ">")
							End If
	
							If glMacWarehouseUser = true then
								If glLogonLinkExempt Then
									Response.Write("<img src='" & GetImagePath("/global/header/mw_login_disabled.gif") & "' border='0' alt='Logon not available on this page' />")
								Else
									Response.Write("<img src='" & GetImagePath("/global/header/mw_login.gif") & "'  border='0' alt='Logon' />")
								End If
							Else
								If glLogonLinkExempt Then
									Response.Write("<img src='" & GetImagePath("/global/header/login_disabled.gif") & "' border='0' alt='Logon not available on this page' />")
								Else
									If GetThisSite = www_cdwg_com And Not glHealthcareUser Then
										Response.Write("<img src='" & GetImagePath("/global/header/g_login.gif") & "' border='0' alt='Logon' />")
									Else
										Response.Write("<img src='" & GetImagePath("/global/header/c_login.gif") & "' border='0' alt='Logon' />")
									End If
								End If
							End If

							If(Not(glLogonLinkExempt)) Then 
								Response.Write("</a>") 
							End If

							Response.Write(_
								"</td>"  &_
								"<td nowrap height=18 valign='middle' align='left'>" &_
									"&nbsp;")

							If(Not(glLogonLinkExempt)) Then
								Response.Write("<a href='https://" & glMainSiteName & glBaseLogonURL & "?site=" & HTMLEncode(glCurrentSite) & "&target=" & HTMLEncode(glCurrentURL) &"' " & LinkStyle & " ")
								If glMacWarehouseUser = true then
									Response.Write("style='FONT-WEIGHT:800;COLOR:#000000'")
								Else
									If GetThisSite = www_cdwg_com And Not glHealthcareUser Then
										Response.Write("style='FONT-WEIGHT:800;COLOR:#99CCFF'")
									Else
										Response.Write("style='FONT-WEIGHT:800;COLOR:#FF9933'")
									End If
								End If
								Response.write( targettop & " title='Log On'>")
							Else
								Response.Write("<span " & LinkStyle & " style='font-weight: 800; COLOR: #CCCCCC;'>")
							End If

							Response.Write("Log On")

							If(Not(glLogonLinkExempt)) Then 
								Response.Write("</a>")
							Else
								Response.Write("</span>")
							End If
							Response.Write("</td>")
						end if
				Response.write(_
				"</tr>"  &_
			"</table>" )
		Else
			Response.write(_
			"<table border='0' bordercolor='#00FFFF' cellpadding='0' height='18' cellspacing='0' " & BGColor & " width='100%' summary='Table for top customer links'>"  &_
				"<tr>" &_
					"<td height='18' nowrap valign='middle' align='right'>" &_
						"<a href='http://" & glMainSiteName & glShoppingCartURL & "' " & LinkStyle & " " & targettop & " title='View Cart'>View Cart</a>" &_
					"</td>" & _
				"</tr>" & _
			"</table>")
		End If
End Sub

'*********************************************************************************************************************
Sub WriteSearchArea(textClass, Hiddencontracts, contractString)
	Dim radiostyle, ImageColor, SearchBtn
		radiostyle	= "style='background:#CC0000'"
		SearchBtn	= "/global/header/search_btn.gif"
		ImageColor	= "orange"

		'4/6/2005 - Paul Haan:  Removed the MW check to change ribbon to blue.
		If (GetThisSite = www_cdwg_com) And Not glHealthcareUser Then 
			ImageColor	= "blue"
		End If

		Response.write(_
		"<table border='0' bordercolor='#0000CC' cellpadding='0' cellspacing='0' height='54' summary='Table for advanced search area'>"  &_
			"<tr>" & _
				"<td class='sst' rowspan='2'>" & _
					"<form name='globalSearch' id='globalSearch' action='http://" & glMainSiteName & "/shop/search/" & GetSearchPage() & "") ' Removed space for defect.

				If glClosedLoop Then 
					Response.Write(contractString)
				End If
				Response.Write(_
					 "' " & targettop & " method='get'>" & _
				"&nbsp;</td>" & _
				"<td valign='top' align='center' rowspan='2'>" &_
					"<table border='0' bordercolor='#00CC00' cellspacing='0' cellpadding='0' width='100%' summary='Search filter table: Mac, PC, All'>" & _
						"<tr>" & _
							"<td colspan='6' align='center' style='padding-bottom: 3px;'>" & _
								"<input type='text' id='key' NAME='key' class='searchbox' maxlength='75' />" &_
								"<input type='hidden' name='sr' value='1'>" &_
							"</td>" & _
						"</tr>" & _
						"<tr>" & _
						
							"<td align='right' class='sstWhite'>")
							If glMacWarehouseUser = true then
								Response.Write("<input id='macsearch' name='platform' type='radio' value='mac' checked " & radiostyle & " />")
							else
								Response.Write("<input id='macsearch' name='platform' type='radio' value='mac' " & radiostyle & " />")
							End If
							Response.Write("<label for='macsearch'>Mac&nbsp;</label>" & _
								"<input id='pcsearch' type='radio' name='platform' value='pc' " & radiostyle & "/>" & _
								"<label for='pcsearch'>PC&nbsp;</label>")
							If glMacWarehouseUser = false then
								Response.Write("<input id='allsearch' type='radio' name='platform' value='all' checked " & radiostyle & "/>")
							else
								Response.Write("<input id='allsearch' type='radio' name='platform' value='all' " & radiostyle & "/>")
							End If
							Response.Write("<label for='allsearch'>All</label>" & _
							"</td>" & _
							
						"</tr>" & _
		  			"</table>" & _
				"</td>")

				If glClosedLoop Then
					Response.Write("<td valign='middle' rowspan='2'>" & Hiddencontracts & "</td>")
				End If

			Response.write(_
				"<td valign='top'>" &_
					"<input type='image' src='" & GetImagePath(SearchBtn) & "' border='0' hspace='7' alt='Search'>" &_
				"</td>"  &_
				"<td valign='top' class='" & textClass & "'>")

				If glClosedLoop Then
					Response.Write("<a href='http://" & glMainSiteName & "/shop/search/default.asp" & contractString & "' " & targettop & " class='" & textClass & "' title='Advanced Search'>")
				Else
					Response.Write("<a href='http://" & glMainSiteName & "/shop/search/default.asp' " & targettop & " class='" & textClass & "' title='Advanced Search'>")
				End If

					Response.write(_
						"<img src='" & GetImagePath("/global/header/AdvancedSearch_" & ImageColor & ".gif") & "' border='0' alt='Advanced Search'/>" &_
					"</a></label>" &_
				"</td>"  &_
			"</tr>"  &_
		"</form></table>")
End Sub

'*********************************************************************************************************************
'Fires OnGetPageConstructor if it exists,
'if not then returns null
function Fire_OnGetPageConstructor()
	if IsFunction("OnGetPageConstructor") then
		call OnGetPageConstructor()
	end if
end function
'*********************************************************************************************************************
'Fires OnGetPageDestructor if it exists,
'if not then returns null
function Fire_OnGetPageDestructor()
	if IsFunction("OnGetPageDestructor") then
		call OnGetPageDestructor()
	end if
end function
'*********************************************************************************************************************
'Fires OnGetClientScripts if it exists,
'if not then returns null
function Fire_OnGetClientScripts()

	if IsFunction("OnGetClientScripts") then
		call OnGetClientScripts()
	end if
end function
'*********************************************************************************************************************
'Fires OnGetPageSettings if it exists,
'if not then returns null
function Fire_OnGetPageSettings()
	if IsFunction("OnGetPageSettings") then
		call OnGetPageSettings()
	end if
end function
'*********************************************************************************************************************
'Fires OnGetMainBodyContent
function Fire_OnGetMainBodyContent()
	if IsFunction("OnGetMainBodyContent") then
		call OnGetMainBodyContent()
	end if
end function
'*********************************************************************************************************************
'Fires OnGetSidebarDataLinks if it exists,
'if not then returns null
function Fire_OnGetSidebarDataLinks(sbArray)
	sbArray = null

	if IsFunction("OnGetSidebarDataLinks") then
		call OnGetSidebarDataLinks(sbArray)
	end if
end function
'*********************************************************************************************************************
'Fires OnGetBottomDataLinks if it exists,
'if not then returns null
function Fire_OnGetBottomDataLinks(bArray)
	bArray = null

	if IsFunction("OnGetBottomDataLinks") then
		call OnGetBottomDataLinks(bArray)
	end if
end function
'*********************************************************************************************************************
'Fires OnGetSidebarContent if it exists,
'if not then returns null
function Fire_OnGetSidebarContent(sbcArray)
	sbcArray = null

	if IsFunction("OnGetSidebarContent") then
		call OnGetSidebarContent(sbcArray)
	end if
end function
'*********************************************************************************************************************
'Fires Fire_OnGetBodyEvents if it exists,
'if not then returns null
function Fire_OnGetBodyEvents(bodyEventsArray)
	bodyEventsArray = null

	if IsFunction("OnGetBodyEvents") then
		call OnGetBodyEvents(bodyEventsArray)
	end if
end function
'*********************************************************************************************************************
'Fires OnGetSubFooter if it exists
'The links that go under the footer on the main page
function Fire_OnGetSubFooter()
	if IsFunction("OnGetSubFooter") then
		call OnGetSubFooter()
	end if
end function
'*********************************************************************************************************************
'	Writes out the Category menu for the header.
	sub WriteCategoryMenu()

	if glShowHeaderOnly Then
		targettop = "target='_top'"
	else
		targettop = ""
	end if

	Response.Write(_
		"<!-- Start Category Menu Code -->" &_
			"<table cellpadding='0' cellspacing='0' border='0' bgcolor='#AAAAAA' width='100%' summary='Main table for category menus'>" &_
				"<tr>" &_
 	  				"<td width='149' valign='top'>" & _
						"<table width='100%' border='0' cellspacing='0' cellpadding='0'>" & _
        					"<tr>" )

						If (GetThisSite = www_cdwg_com) OR (GetThisSite() = www_cdw_ca) Then
							Response.write(_
								"<td width='149' valign='top' align='right' bgcolor='#000000' nowrap>" & _
									"<img src='" & GetImagePath("/global/pixels/black.gif") & "' width='1' height='26' />" & _
									"<img src='" & GetImagePath("/global/header/navBar/C_Span.gif") & "' width='148' height='26' />" & _
								"</td>" )
						Else
							If glMacWarehouseUser = True Then
								If glClosedLoop = true OR glTrueClosedLoop = True Then
									Response.write(_
										"<td width='149' valign='top' nowrap align='right' bgcolor='#000000'>" & _
											"<img src='" & GetImagePath("/global/pixels/black.gif") & "' width='1' height='26' />" & _
											"<img src='" & GetImagePath("/global/header/navBar/MW_Span.gif") & "' width='148' height='26' />" & _
										"</td>" )
								Else
									Response.write(_
										"<td width='149' valign='top'>" & _
											"<img src='" & GetImagePath("/global/header/navBar/MW_Left.gif") & "' width='149' height='26' />" & _
										"</td>" )
								End If
							Else
								If glClosedLoop = true OR glTrueClosedLoop = True Then
									Response.write(_
										"<td width='149' valign='top' align='right' nowrap  bgcolor='#000000'>" & _
											"<img src='" & GetImagePath("/global/pixels/black.gif") & "' width='1' height='26' />" & _
											"<img src='" & GetImagePath("/global/header/navBar/C_Span.gif") & "' width='148' height='26' />" & _
										"</td>" )
								Else
									Response.write(_
										"<td width='149' valign='top'><img src='" & GetImagePath("/global/header/navBar/C_Left.gif") & "' width='149' height='26' /></td>" )
								End If
							End If
						End If
	Response.write(_
        					"</tr>" & _
      					"</table>" & _
					"</td>"  &_
					"<td>" )

				If glClosedLoop = False Then
					If glMacWarehouseUser = True Then
						MenuSubs_InsertJavascript MenuSubs_CdwMacMenu, False
						MenuSubs_GenerateTabs MenuSubs_CdwMacMenu
					ElseIf GetThisSite() = www_cdwg_com Then
						MenuSubs_InsertJavascript MenuSubs_CdwgMenu, False
						MenuSubs_GenerateTabs MenuSubs_CdwgMenu
					ElseIf GetThisSite() = www_cdw_ca Then
						MenuSubs_InsertJavascript MenuSubs_CdwCanadaMenu, False
						MenuSubs_GenerateTabs MenuSubs_CdwCanadaMenu
					Else
						MenuSubs_InsertJavascript MenuSubs_CdwMenu, False
						MenuSubs_GenerateTabs MenuSubs_CdwMenu
					End If
				Else
					If glMacWarehouseUser = True Then
						Response.write("<img width='100%' height='24' src='" & GetImagePath("/macwarehouse/gui/bevelbar_left.gif") & "' alt='' />")
					Else
						Response.write("<img width='100%' height=1 src='" & GetImagePath("/global/pixels/none.gif") & "' alt='' />" )
					End If
				End If

		Response.write(_
			"</td>" &_
			"<td width='100%' nowrap valign='middle' align='right' class='num'  style='font-weight:800; ")

				If glMacWarehouseUser = true Then
					Response.write("color:#666666;' bgcolor='#AAAAAA' background='" &  GetImagePath("/global/header/navBar/MW_Span.gif") & "'")
				Else
					Response.write("' background='" & GetImagePath("/global/header/navBar/C_Span.gif") & "' bgcolor='#AAAAAA' ")
				End If

		Response.write(_
			">" )

				If Len(CurPhoneNumber) > 0 Then
					Response.write(HTMLEncode(CurPhoneNumber))
				Else
					Response.write(OnGetPhoneNumber())
				End If

		Response.write(_
			"&nbsp;&nbsp;</td>" & _
			"<td width='1' valign='top' align='right' bgcolor='#000000'>" & _
				"<img src='" & GetImagePath("/global/pixels/none.gif") & "' width='1' height='26' />" & _
			"</td>" &_
		"</tr>"   &_
	"</table>" )

	Response.Write("<!-- End Category Menu Code -->"  )
	end sub
'*********************************************************************************************************************
'	Writes out the Top sidebar for the main table.
sub writeSidebarDataLinks(TsideArr)
'	Write the array of links and the red link at the bottom, if the array exists
	if isArray(TsideArr) then
		Response.Write  "<!-- Start Top Sidebar Code -->" 
		dim ts, textcolor
		Dim urlTitle
		textcolor = "#000000"

		for ts = lBound(TsideArr) to uBound(TsideArr)
			if Not(isEmpty(TsideArr(ts,0))) and Not(isNull(TsideArr(ts,0))) and len(TsideArr(ts,0)) > 0 then
				Response.Write "<table border='0' cellpadding='0' cellspacing='0' valign='top' bordercolor='red' summary='Extra Sidebar links'>"
				if isNull(TsideArr(ts,1)) or len(TsideArr(ts,1)) = 0 or isEmpty(TsideArr(ts,1)) then
					Response.Write (_
					"<tr>"   &_
						"<td colspan='3'>" &_
							"<img src='" & GetImagePath(glLabelPath & TsideArr(ts,0) & ".gif") & "'  alt='" & TsideArr(ts,0) & "' />" &_
						"</td>" &_
					"</tr>"  )
				else
					Response.Write (_
						"<tr>"   &_
							"<td width='5' valign='middle' align='left'>" &_
								"<img width='5' height='1' src='" & GetImagePath("/global/pixels/none.gif") & "' alt='' />" &_
							"</td>"   &_
							"<td width='10' align='left' valign='top' class='MenuLink'>" &_
								"&#183;</td>"   &_
							"<td width='125' valign='top'>")

								If UBound(TsideArr, 2) = 3 Then
									If IsNull(TsideArr(ts,3)) Or IsEmpty(TsideArr(ts,3)) Or Len(TsideArr(ts,3)) = 0 Then
										urlTitle = TsideArr(ts,0)
									Else
										urlTitle = TsideArr(ts,3)
									End If
								Else
									urlTitle = TsideArr(ts,0)
								End If
								
								if TsideArr(ts,2) then
									Response.Write ("<a href='javascript:NewPopupWindow(""" & HTMLEncode(TsideArr(ts,1)) & """)' CLASS='SidebarLink' title='" & urlTitle & "  (Popup Window)'>")
								else
									Response.Write ("<a href='" & HTMLEncode(TsideArr(ts,1)) & "' CLASS='SidebarLink' title='" & urlTitle & "'>")
								end if

								Response.Write (_
										TsideArr(ts,0) & "</A><br />" &_
									"</td>"   &_
								"</tr>"   &_
								"<tr>"   &_
									"<td colspan='3'>" &_
										"<img width='1' height='3' src='" & GetImagePath("/global/pixels/none.gif") & "' alt='' />" &_
									"</td>"   &_
								"</tr>"  )
				end if
				Response.write("</table>"  )
			end if
		next

		Response.write ("<img src='" & GetImagePath("/global/pixels/none.gif") & "' height='16' width='1' alt='' /><br />" )
		Response.Write ("<!-- End Top Sidebar Code -->"  )
	else 
		if Len(TsideArr) > 0 Then Response.write(TsideArr) End If
	end if
end sub
'*********************************************************************************************************************
'	Writes out the Bottom sidebar for the main table.
'	These will generally be used only for the inside sections.
sub writeBottomDataLinks(BsideArr)
'	Write the array of links at the bottom, if the array exists
	if isArray(BsideArr) then
		Response.Write   "<!-- Start Bottom Sidebar Code -->" 
		dim b

		for b = lBound(BsideArr) to uBound(BsideArr)
			if Not(isEmpty(BsideArr(b,0))) and Not(isNull(BsideArr(b,0))) and len(BsideArr(b,0)) > 0 then
				Response.Write "<table border='0' cellpadding='0' cellspacing='0' valign='top' bordercolor='red' summary='" & Trim(BsideArr(b,0)) & "'>"
				if isNull(BsideArr(b,1)) or len(BsideArr(b,1)) = 0 or isEmpty(BsideArr(b,1)) then
					Response.Write (_
					"<tr>"   &_
						"<td colspan='3'>" &_
							"<img src='" & GetImagePath(glLabelPath & Trim(BsideArr(b,0)) & ".gif") & "'  alt='" & Trim(BsideArr(b,0)) & "' />" &_
						"</td>"   &_
					"</tr>"  )
				else
					Response.Write (_
						"<tr>"   &_
							"<td width='5' valign='middle' align='left'>" &_
								"<img width='5' height='1' src='" & GetImagePath("/global/pixels/none.gif") & "' alt='' />" &_
							"</td>" &_
							"<td width=10 align='left' valign='top' class='SidebarLink'>&#183;</td>" &_
							"<td width=123 valign='top'>")

						if BsideArr(b,2) then
							Response.Write ("<a href='javascript:NewPopupWindow(""" & HTMLEncode(BsideArr(b,1)) & """)' CLASS='MenuLink' title='" & BsideArr(b,0) & "  (Popup Window)'>")
						else
							Response.Write ("<a href='" & HTMLEncode(BsideArr(b,1)) & "' CLASS='SidebarLink' title='" & BsideArr(b,0) & "'>")
						end if

						Response.Write (_
										BsideArr(b,0) & "</A><br />" &_
									"</td>"  &_
								"</tr>"  &_
								"<tr>"  &_
									"<td colspan='3'>" &_
										"<img width='1' height='3' src='" & GetImagePath("/global/pixels/none.gif") & "' alt='' />" &_
									"</td>"   &_
								"</tr>"  )
				end if
				Response.write("</table>"  )
			end if
		next

	Response.write ("<img src='" & GetImagePath("/global/pixels/none.gif") & "' height='16' width='1' alt='' /><br />"  )
	Response.Write ("<!-- End Bottom Sidebar Code -->"  )
	end if
end sub
'*********************************************************************************************************************
'	Writes out the sidebar content table.
sub writeSidebarContent(sidecontentArr)
	dim c
'	Write the array of links and the red link at the bottom, if the array exists
	if isArray(sidecontentArr) then
	Response.Write ( "<!-- Start Sidebar Content Code -->" )
		for c = lBound(sidecontentArr) to uBound(sidecontentArr)
		if Not(isEmpty(sidecontentArr(c,0))) and Not(isNull(sidecontentArr(c,0))) and len(sidecontentArr(c,0)) > 0 then
			if sidecontentArr(c,0) = 0 then				'this section is for an image w/o text
				Response.write (_
					"<table cellpadding='0' cellspacing='0' border='0' Summary='" & sidecontentArr(c,3) & "'>" &_
						"<tr>" &_
							"<td width='138' align='center'>")
								if sidecontentArr(c,2) = "" or isnull(sidecontentArr(c,2)) then
									response.write "<img src='" & HTMLEncode(sidecontentArr(c,1)) & "' border='0' alt='" & sidecontentArr(c,3) & "' />"
								else
									Response.Write	"<a href='" & HTMLEncode(sidecontentArr(c,2)) & "'><img src='" & HTMLEncode(sidecontentArr(c,1)) & "' border='0' alt='" & sidecontentArr(c,3) & "' /></a><br />"
								end if
				Response.write("</td></tr></table>")
			elseif sidecontentArr(c,0) = 1 then				'this section is for a text area (w/header)
				Response.write(_
				"<table width='138' cellpadding='0' cellspacing='0' border='0' summary='Table for a text area (w/header)'>" &_
					"<tr>" &_
						"<td valign='top' class='btsbRed'>" &_
							sidecontentArr(c,1) & "<br />" &_
							"<img width='135' height='6' src='" & GetImagePath("/global/lines/gydot135x6.gif") & "' border='0' alt='' /><br /></td></tr>" &_
					"<tr><td valign='top' class='SBText'>" &_
							 sidecontentArr(c,2) & "<br/></td></tr>" &_
				"</table>")
			elseif sidecontentArr(c,0) = 2 then				'this section is for text next to an arrow
				response.write(_
					"<table border='0' width='138' cellpadding='0' cellspacing='0' summary='Table for text next to an arrow'>" &_
						"<tr>" &_
							"<td width='15' align='center' valign='top'><a href='"& HTMLEncode(sidecontentArr(c,2)) &"'><img src='" & GetImagePath("/global/arrows/red_arrow.gif") & "' alt='' border='0' /></a></td>" &_
							"<td width='123' valign='top'><a href='"& HTMLEncode(sidecontentArr(c,2)) &"' class='SidebarLink' title='" & sidecontentArr(c,1) & "'>" & sidecontentArr(c,1) & "</a></td>" &_
						"</tr>" &_
					"</table>" )
			elseif sidecontentArr(c,0) = 3 then				'this section is for an image with text below it
				response.write(_
					"<table border='0' width='138' cellpadding='0' cellspacing='0' summary='Table for an image with text below it'>" &_
						"<tr>" &_
							"<td>" & _
								"<a href='" & HTMLEncode(sidecontentArr(c,2)) & "'><img src='" & HTMLEncode(sidecontentArr(c,1)) & "' border='0' alt='" & sidecontentArr(c,1) & "' /></a>" & _
								"<br />" &_
								"<a href='" & HTMLEncode(sidecontentArr(c,2)) & "' class='sstLink'>" & sidecontentArr(c,3) & "</a>" & _
							"</td>" &_
						"</tr>" &_
					"</table>"  )
			end if
				Response.write ("<img src='" & GetImagePath("/global/pixels/none.gif") & "' height='16' width='1' alt='' /><br />" )
		end if
		next
			Response.Write ("<!-- End Sidebar Content Code -->"  )
	end if
end sub
'********************************************************************************************************************
'Writes out the Custom Page Link
sub WriteCustomPageLink()
	Response.Write(_
		 "<!-- Start Custom Page Link Code -->"  &_
			"<table width='138' border='0' cellpadding='0' cellspacing='0' valign='top' Summary='Table for Custom Page Link'>"  &_
				"<tr>"  &_
					"<td colspan='3'>")
						If GetThisSite = www_cdwg_com Then
							Response.write("<img src='" & GetImagePath(glLabelPath & "Prem_Page.gif") & "'  alt='Custom Page' />")
						Else
							Response.write("<img src='" & GetImagePath(glLabelPath & "CustomPage.gif") & "'  alt='Custom Page' />")
						End If

	Response.write(_
					"</td>"  &_
				"</tr>" )

		Response.Write(_
				"<tr>"  &_
					"<td width='5' valign='middle' align='left'><img width='5' height='1' src='" & GetImagePath("/global/pixels/none.gif") & "' alt='' /></td>"  &_
					"<td width='10' align='left' valign='top' class='MenuLink'>&#183;</td>"  &_
					"<td width='123' valign='top'>")
						If UserLoggedInSecure Then
							Response.write("<a href='https://" & glMainSiteName & "/shop/custompages/default.aspx?CustomPageKey=" & HTMLEncode(GetCookieValue(eaCookieCustomPage)) & "'")
						Else
							Response.write("<a href='http://" & glMainSiteName & "/shop/custompages/default.aspx?CustomPageKey=" & HTMLEncode(GetCookieValue(eaCookieCustomPage)) & "'")
						End If

		Response.write(_
						"class='SidebarLink' title='" & glCustomPageTitle & "'>" &  glCustomPageTitle & "</A><br />" &_
					"</td>"  &_
				"</tr>"  &_
				"<tr>"  &_
					"<td colspan='3'>" &_
						"<img width='1' height='3' src='" & GetImagePath("/global/pixels/none.gif") & "' alt='' />" &_
					"</td>"  &_
				"</tr>"  &_
			"</table>"   &_
			"<img src='" & GetImagePath("/global/pixels/none.gif") & "' height='16' width='1' alt='' /><br />"  & _
			"<!-- End Custom Page Link Code -->"  )
End Sub
'********************************************************************************************************************
'Writes out the resources bar
sub WriteResourcesBar()
	Response.Write ( "<!-- Start Resource Sidebar Code -->" )
		dim bs, textcolor, ResourceString
		textcolor = "#000000"

		'Call in the include files
		If GetThisSite() = www_cdwg_com and glHealthcareUser Then
			ResourceString	= GetContentFileFromCache("Sidebar_ResourcesHC", "/includes/side_bars/ResourcesHC.html")
		ElseIf GetThisSite() = www_cdwg_com Then
			ResourceString	= GetContentFileFromCache("Sidebar_Resources", "/includes/side_bars/ResourcesG.html")
		ElseIf glMacWarehouseUser = True Then
			ResourceString	= GetContentFileFromCache("Sidebar_ResourcesMW", "/includes/side_bars/ResourcesMW.html")
		ElseIf glCDWCanadaUser = True Then
			ResourceString	= GetContentFileFromCache("Sidebar_Resources", "/includes/side_bars/ResourcesCA.html")
		Else
			ResourceString	= GetContentFileFromCache("Sidebar_Resources", "/includes/side_bars/Resources.html")
		End If

		Response.write(ResourceString)

		Response.write(_
			"<img src='" & GetImagePath("/global/pixels/none.gif") & "' height='16' width='1' alt='' /><br />")
		'Give them a way to escape Healthcare
		If glWebSiteKey = www_cdwg_com And glHealthcareUser And Not UserIsLoggedIn() Then
			Response.Write(_
				"<table width='138' border='0' cellpadding='0' cellspacing='0' valign='top'>"  &_
					"<tr>" &_
						"<td>" &_
							"<a href='/changevm.asp'><img src='" & GetImagePath(glLabelPath & "cdwg_com.gif") & "' border='0' alt='Back to CDWG.com'></a>" &_
						"</td>"  &_
					"</tr>" &_
				"</table>" &_
		   		"<img src='" & GetImagePath("/global/pixels/none.gif") & "' height='16' width='1' alt='' /><br />") 
		End If
		If glWebSiteKey = www_cdwg_com And Not UserIsLoggedIn() Then
			Response.Write(_
				"<table width='138' border='0' cellpadding='0' cellspacing='0' valign='top'>"  &_
					"<tr>" &_
						"<td>" &_
							"<a href='http://" & GetSiteName(www_cdw_com) & "/'><img src='" & GetImagePath(glLabelPath & "cdwcom.gif") & "' border='0' alt='CDW.com'></a>" &_
						"</td>"  &_
					"</tr>" &_
				"</table>" &_
		   		"<img src='" & GetImagePath("/global/pixels/none.gif") & "' height='16' width='1' alt='' /><br />") 
		End If
		Response.Write("<!-- End Resource Sidebar Code -->")
end sub
'********************************* Write the footer *********************************
sub WriteFooter()
	Dim FilePath, TagLinePath, FooterLinksFile, FooterLinksCacheName

	TagLinePath = "/cdw/gui/footer.gif"
	FilePath	= "http://" & glMainSiteName & "/webcontent/inside/"

	'4/6/2005 - Paul Haan:  Removed the MW check to change footer to blue.
	If GetThisSite = www_cdwg_com Then
		FilePath	= "http://" & glMainSiteName & "/webcontent/insideG/"
		If glHealthcareUser Then
			TagLinePath = "/cdw/gui/footer.gif"
		Else
			TagLinePath = "/cdwg/gui/footer.gif"
		End If
		FooterLinksCacheName = "FooterLinks_G"
	ElseIf glCDWCanadaUser Then
		FilePath	= "http://" & glMainSiteName & "/webcontent/insideCA/"
		FooterLinksCacheName = "FooterLinks_CA"
	Else
		FilePath	= "http://" & glMainSiteName & "/webcontent/inside/"
		FooterLinksCacheName = "FooterLinks_C"
	End If

	FooterLinksFile = GetContentFileFromCache(FooterLinksCacheName, "/includes/FooterLinks.html")

	if glFlagShowMainHeader then
		Response.Write  "<!-- Start Footer Code -->" 

		Response.write(_
		"<table border='0' height='29' cellspacing='0' cellpadding='0' summary='Table for footer' style='padding-bottom:5px; padding-top: 7px;'>" & _
			"<tr>" & _
				"<td width='150' valign='top'>" & _
					"<img src='" & GetImagePath(TagLinePath) & "' alt='The Right Technology, Right Away' />" &_
				"</td>")

					If glMacWarehouseUser Then
						Response.write("<td><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='12' height='1' alt='' /></td>")
					Else
						Response.write("<td><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='7' height='1' alt='' /></td>")
					End IF

		Response.write(_
				"<td>" & _
					"<table border='0' height='29' cellspacing='0' cellpadding='0' summary='Table for footer' style='padding-bottom:5px;'>" & _
						"<tr>" & _
							"<td>" & _
								FooterFile & _
							"</td>" & vbCrLf &_
						"</tr>" & _
						"<tr>")

						If glTrueClosedLoop <> true then
							Response.write(_
								"<td class='sst' valign='center'>" &_
									"<span class='bts'>")

										If GetThisSite = www_cdwg_com Then
											Response.write("Copyright " & Year(Date) & " CDW Government, Inc.<br />")
										ElseIf glCDWCanadaUser Then
											Response.write("Copyright " & Year(Date) & " CDW Canada Inc.<br />")
										Else
											Response.write("Copyright " & Year(Date) & " CDW Corporation<br />")
										End If

							Response.write(_
									"</span>")
								if glVendorSessionUser AND Len(VendorFooterFile) > 0 THEN
									Response.write(VendorFooterFile)
								Else
									Response.write(FooterLinksFile)
								End If

							Response.write(_
								"</td>" )
						end if
	end if
		Response.write("</tr></table>")

		Response.write(_
				"</td>" & _
			"</tr>" & _
		"</table>")

	'Look for information to go below the footer...Example is on the main page
	Call Fire_OnGetSubFooter()
	Response.Write vbCrLf & "<!-- End Footer Code -->"  
end sub

'*******************************************************************************************************************************
'Write out the javascript BODY event functions
sub writeBodyEvents
	dim eventArray, x, y
	dim functionName, tempArray

	' Write a marker variable for the menus to know when the page is loaded,
	' and the menus are safe to use.
	Response.Write(vbcrlf & "var DDM_IsLoaded = false;" & vbcrlf)

	call Fire_OnGetBodyEvents(eventArray)

	Response.Write(_
		 	"function bodyOnClickConstructor()" &_
		 	"{" )

	if isArray(eventArray) then
		for x = lBound(eventArray) to uBound(eventArray)
			if eventArray(x,0) = glBodyOnClick then
				tempArray = eventArray(x,1)

				if isArray(tempArray) then
					for y = lBound(tempArray) to uBound(tempArray)
						Response.Write tempArray(y)
					next
				end if
			end if
		next
	end if
	Response.Write "}"

	Response.Write(_
		 	"function bodyOnLoadConstructor()" &_
		 	"{")

	if isArray(eventArray) then
		for x = lBound(eventArray) to uBound(eventArray)
			if eventArray(x,0) = glBodyOnLoad then
				tempArray = eventArray(x,1)

				if isArray(tempArray) then
					for y = lBound(tempArray) to uBound(tempArray)
						Response.Write tempArray(y)
					next
				end if
			end if
		next
	end if
	Response.Write "}"

	Response.Write(_
		 	"function bodyOnUnloadConstructor()" &_
		 	"{" )

	if isArray(eventArray) then
		for x = lBound(eventArray) to uBound(eventArray)
			if eventArray(x,0) = glBodyOnUnload then
				tempArray = eventArray(x,1)

				if isArray(tempArray) then
					for y = lBound(tempArray) to uBound(tempArray)
						Response.Write tempArray(y)
					next
				end if
			end if
		next
	end if
	Response.Write "}"
end sub
'*******************************************************************************************************************************
'Returns the browser's version number
function GetBrowserVersion()
	dim userAgent, version

	userAgent = Request.ServerVariables("HTTP_USER_AGENT")
	version = 0

	if inStr(userAgent, "MSIE") then
		'Internet Explorer
		version = cint(mid(userAgent, inStr(userAgent,"MSIE") + 5, 1))
	elseif inStr(useragent, "Mozilla") then
		'Netscape Compatible
		if inStr(userAgent, "compatible;") = 0 then
			'Netscape Navigator
			version = cint(mid(userAgent, inStr(userAgent, "/") + 1, 1))
		else
			'Other type of browser
			version = cint(mid(userAgent, inStr(userAgent, "compatible;") + 12))
		end if
	end if

	GetBrowserVersion = version
end function
'******************************************************************************************************************
'Returns true if browser is IE
function IsBrowserIE()
	dim userAgent, returnValue

	userAgent = Request.ServerVariables("HTTP_USER_AGENT")

	if inStr(userAgent,"MSIE") then
		returnValue = true
	else
		returnValue = false
	end if

	IsBrowserIE = returnValue
end function
'******************************************************************************************************************************
'	DESCRIPTION:
'		Draws the page errors.
Sub DrawPageErrors()

	Dim x
	If(Not(IsEmpty(RunTimeErrors)) and Not(IsNull(RunTimeErrors)) and IsNumeric(RunTimeErrors)) Then
	If(glValidationFormat = "global") Then
		Response.Write(_
			"<!-- Start Errors -->"  &_
			"<table width='100%' bgcolor='#EFEFEF' cellpadding='9' cellspacing=0 bordercolor='#ff00ff' border='0' summary='outside error message table'>"  &_
				"<tr>"  &_
					"<td width='100%'>"  & _
						"<table width='100%' cellpadding='0' cellspacing='0' border='0' summary='Main error message table'>"  &_
							"<tr>"  &_
								"<td>" )
								Dim MainErrorText
								If(IsArray(MainErrors)) Then
									MainErrorText = MainErrors(RunTimeErrors,0)
								End If

								Dim AltErrorText, ErrorImageIcon, InstructionText
								If CInt(RunTimeErrors) = errInvalid Then
									AltErrorText	= "Critical Error"
									ErrorImageIcon	= "x_icon"
									InstructionText	= "Please correct the following:"
								ElseIf CInt(RunTimeErrors) = errMissing Then
									AltErrorText	= "Missing Information"
									ErrorImageIcon	= "question_icon"
									InstructionText	= "Please correct the following:"
								ElseIf CInt(RunTimeErrors) = errGeneral Then
									AltErrorText	= "General Error"
									ErrorImageIcon	= "exclamation_icon"
									InstructionText	= "Please correct the following:"
								ElseIf CInt(RunTimeErrors) = errSearch Then
									AltErrorText	= "Search Error"
									ErrorImageIcon	= "i_icon"
									InstructionText	= "Please note:"
								ElseIf CInt(RunTimeErrors) = errSiteTransfer Then
									AltErrorText	= "Site Transfer"
									ErrorImageIcon	= "site_trans_icon"
									InstructionText	= "Please note:"
								ElseIf CInt(RunTimeErrors) = errGeneralSpanish Then
									AltErrorText	= "Error General"
									ErrorImageIcon	= "exclamation_icon"
									InstructionText	= "Por favor corrija los siguientes datos:"
								ElseIf CInt(RunTimeErrors) = errInvalidSpanish Then
									AltErrorText	= "Petici&#243;n inv&#225;lida"
									ErrorImageIcon	= "x_icon"
									InstructionText	= "Por favor corrija los siguientes datos:"
								ElseIf CInt(RunTimeErrors) = errSessionTimeout Then
									AltErrorText	= "Your session has expired"
									ErrorImageIcon	= "exclamation_icon"
									InstructionText	= ""
								End If

								Dim ErrorTemplate
								ErrorTemplate = "?layer=comp&wid=600&hei=46&fmt=gif&quantize=adaptive,diffuse,256,&op_sharpen=0&resMode=sharp&op_usm=0.0,0.0,0.0,1&iccEmbed=0&$Text=" & MainErrorText & "&$layer_1_src=CDW/" & ErrorImageIcon

								Response.Write("<img src='" & GetScene7Path("error_template") & ErrorTemplate & "' border='0' alt='" & AltErrorText & "' /><br />")
		Response.Write(_
								"</td>"  &_
							"</tr>"  &_
						"</table>"  )

		Response.Write(_
						"<table width='100%' cellpadding='0' cellspacing='0' border='0' summary='Main error message table # 2'>"  &_
							"<tr>" &_
								"<td width='45' align='left'>" &_
									"<img src='" & GetImagePath("img.cdw.com/global/pixels/none.gif") & "' width='45' height='1' border='0' /></td>" &_
								"<td align='left'>")

									If(IsNumeric(RunTimeErrors)) Then
										If(IsArray(MainErrors)) Then
											Response.Write(MainErrors(RunTimeErrors,1))
										End If
									End If

		Response.Write(_
								"</td>"  &_
							"</tr>"  &_
							"<tr>"  &_
								"<td colspan='2'>" &_
									"<img src='" & GetImagePath("/global/pixels/none.gif") & "' width='1' height='5' border='0' /><br />" &_
								"</td>"  &_
							"</tr>"  &_
						"</table>"  )

				If(IsArray(RunTimeSubErrors)) Then
					Response.Write(_
						"<table width='100%' cellpadding='0' cellspacing='0' border='0' summary='Sub-error message table'>"  &_
							"<tr>"  &_
								"<td>" &_
									"<hr color='#C0C0C0' size='1' noShade='noshade'></hr>" &_
								"</td>"  &_
							"</tr>"  &_
							"<tr>"  &_
								"<td>" & InstructionText & "<br /></td>" &_
							"</tr>"  &_
							"<tr>"  &_
								"<td>" &_
									"<img src='" & GetImagePath("/global/pixels/none.gif") & "' width='1' height='18' border='0' /><br />" &_	
								"</td>"  &_
							"</tr>"  &_
							"<tr>"  &_
								"<td width='100%'>" &_
									"<ul>")

										For x = Lbound(RunTimeSubErrors, 1) To Ubound(RunTimeSubErrors, 1)
											If(IsNumeric(RunTimeSubErrors(x))) Then
												If(IsArray(SubErrors)) Then
													Response.Write("<li>" & SubErrors(RunTimeSubErrors(x)) & "</li>")
												End If
											End If
										Next

					Response.Write(_
									"</ul>" &_
								"</td>"  &_
							"</tr>"  &_
						"</table>" )
				End If

		Response.Write(_
					"</td>"  &_
				"</tr>"  &_
			"</table>"  & _
			"<!-- End Errors -->"  )
			'For Signup page the error message format starts here
			Else
			Response.Write(_
			"<!-- Start Errors -->"  &_
			"<table width='100%'  cellpadding='9' cellspacing=0  border='0' summary='outside error message table'>"  &_
				"<tr>"  &_
					"<td width='100%'>"  & _
						"<table width='100%' cellpadding='0' cellspacing='0' border='0' summary='Main error message table'>"  &_
							"<tr>"  &_
								"<td>" )								
								If(IsArray(MainErrors)) Then
									MainErrorText = MainErrors(RunTimeErrors,0)
								End If	
								
								MainErrorText = "Please Correct the following:"
												
								If CInt(RunTimeErrors) = errInvalid Then
									AltErrorText	= "Please correct the following:"
									ErrorImageIcon	= "exclamation_icon"
									InstructionText	= "Please correct the following:"
								ElseIf CInt(RunTimeErrors) = errMissing Then
									AltErrorText	= "Missing Information"
									ErrorImageIcon	= "question_icon"
									InstructionText	= "Please correct the following:"
								ElseIf CInt(RunTimeErrors) = errGeneral Then
									AltErrorText	= "General Error"
									ErrorImageIcon	= "exclamation_icon"
									InstructionText	= "Please correct the following:"
								ElseIf CInt(RunTimeErrors) = errSearch Then
									AltErrorText	= "Search Error"
									ErrorImageIcon	= "i_icon"
									InstructionText	= "Please note:"
								ElseIf CInt(RunTimeErrors) = errSiteTransfer Then
									AltErrorText	= "Site Transfer"
									ErrorImageIcon	= "site_trans_icon"
									InstructionText	= "Please note:"
								ElseIf CInt(RunTimeErrors) = errGeneralSpanish Then
									AltErrorText	= "Error General"
									ErrorImageIcon	= "exclamation_icon"
									InstructionText	= "Por favor corrija los siguientes datos:"
								ElseIf CInt(RunTimeErrors) = errInvalidSpanish Then
									AltErrorText	= "Petici&#243;n inv&#225;lida"
									ErrorImageIcon	= "x_icon"
									InstructionText	= "Por favor corrija los siguientes datos:"
								ElseIf CInt(RunTimeErrors) = errSessionTimeout Then
									AltErrorText	= "Your session has expired"
									ErrorImageIcon	= "exclamation_icon"
									InstructionText	= ""
								End If
						
								Response.Write("<div class='errorNotification'>")							
								
								If CInt(RunTimeErrors) = errInvalid Then
								Response.Write("<img src='" & GetImagePath("/assets/pages/checkout/error-alert-icon.jpg") & ErrorTemplate &  "' border='0'  alt='" & AltErrorText & "' />")
								ElseIf CInt(RunTimeErrors) = errSiteTransfer Then
								Response.Write("<img src='" & GetImagePath("/assets/icons/notification-warning_large.gif") & ErrorTemplate &  "' border='0'  alt='" & AltErrorText & "' />")
								End If
								Response.Write("<h4>" & AltErrorText & "</h4>")
								
								If(IsArray(RunTimeSubErrors)) Then
								Response.Write("<ul class='errorList'>")
								For x = Lbound(RunTimeSubErrors, 1) To Ubound(RunTimeSubErrors, 1)
											If(IsNumeric(RunTimeSubErrors(x))) Then
												If(IsArray(SubErrors)) Then
													Response.Write("<li>" & SubErrors(RunTimeSubErrors(x)) & "</li>")
												End If
											End If
										Next
										Response.Write(_
									"</ul>" &_
								"</div")
		Response.Write(_
								"</td>"  &_
							"</tr>"  &_							
						"</table>"  )
		Response.Write(_
					"</td>"  &_
				"</tr>"  &_
			"</table>"  & _
			"<!-- End Errors -->"  )
			End If
		End If
		End If
	End Sub
'******************************************************************************************************************
'*	Below are the functions to fill in the sidebar links for specific sections									  *
'******************************************************************************************************************
' Write the array for the Support Links
Function Get_ServiceLinks(sbarray)

	Dim supportCDWSite
	dim dataArray(5,2)

		dataArray(0,0) = "ServiceLinks"
		dataArray(0,1) = ""
		dataArray(0,2) = False

		If glCDWCanadaUser Then
			dataArray(1,0) = "Contact Us"
			dataArray(1,1) = "http://" & glMainSiteName & "/webcontent/insideCA/contact.asp"
			dataArray(1,2) = False
		Else
			dataArray(1,0) = "Contact Us"
			dataArray(1,1) = "http://" & glMainSiteName & "/webcontent/inside/contact/default.asp"
			dataArray(1,2) = False
		End If

		dataArray(2,0) = "Product Returns"
		dataArray(2,1) = "http://" & glMainSiteName & "/webcontent/inside/service/returns.asp"
		dataArray(2,2) = False
		dataArray(3,0) = "Cancel order"
		dataArray(3,1) = "http://" & glMainSiteName & "/webcontent/inside/service/cancel_order.asp"
		dataArray(3,2) = False
		If glCDWCanadaUser = False Then
			dataArray(4,0) = "Catalogs &amp; E-mail"
			dataArray(4,1) = "http://" & glMainSiteName & "/webcontent/inside/service/mail_list.asp"
			dataArray(4,2) = False
			dataArray(5,0) = "Locate Stores"
			dataArray(5,1) = "http://" & glMainSiteName & "/webcontent/inside/help/maps.asp"
			dataArray(5,2) = False
		End If
	sbArray = dataArray
End Function

' Write the array for the My Company Links
Function Get_MyCompanyLinks(sbarray)
	If GetThisSite() = www_cdwg_com and glHealthcareUser Then
		sbarray	= GetContentFileFromCache("Sidebar_MyCompanyHC", "/includes/side_bars/MyCompanyHC.html")
	ElseIf GetThisSite() = www_cdwg_com Then
		sbarray	= GetContentFileFromCache("Sidebar_MyCompany", "/includes/side_bars/MyCompanyG.html")
	ElseIf glMacWarehouseUser = True Then
		sbarray	= GetContentFileFromCache("Sidebar_MyCompanyMW", "/includes/side_bars/MyCompanyMW.html")
	ElseIf glCDWCanadaUser = True Then
		sbarray	= GetContentFileFromCache("Sidebar_MyCompany", "/includes/side_bars/MyCompanyCA.html")
	Else
		sbarray	= GetContentFileFromCache("Sidebar_MyCompany", "/includes/side_bars/MyCompany.html")
	End If
End Function

' Write the array for the MyAccount Links
function Get_MyAccountLinks(sbArray)
	If GetThisSite() = www_cdwg_com and glHealthcareUser Then
		sbarray	= GetContentFileFromCache("Sidebar_MyAccountHC", "/includes/side_bars/MyAccountHC.html")
	ElseIf GetThisSite() = www_cdwg_com Then
		sbarray	= GetContentFileFromCache("Sidebar_MyAccount", "/includes/side_bars/MyAccountG.html")
	ElseIf glMacWarehouseUser = True Then
		sbarray	= GetContentFileFromCache("Sidebar_MyAccountMW", "/includes/side_bars/MyAccountMW.html")
	ElseIf glCDWCanadaUser = True Then
		sbarray	= GetContentFileFromCache("Sidebar_MyAccount", "/includes/side_bars/MyAccountCA.html")
	Else
		sbarray	= GetContentFileFromCache("Sidebar_MyAccount", "/includes/side_bars/MyAccount.html")
	End If
end function

function Get_SupportLinks(bArray)
	dim dataArray(7,2)
	Dim UserKey, UserID, OrganizationID
	Dim Extranet, EppUser

	Extranet = False

	If(UserIsLoggedIn or UserHasSavedCredentials) Then
		UserKey = GetUserKey()
		UserID = GetUserID(UserKey)
		OrganizationID = GetUserOrganizationID(UserID)
'		EppUser = UserHasEPP(UserID)
		Extranet = OrganizationHasProgram(OrganizationID, GetProgramID(pgExtranet))
	End If

	dataArray(0,0) = "SupportLinks"
	dataArray(0,1) = ""
	dataArray(0,2) = False
	dataArray(1,0) = "My Support Cases"
	dataArray(1,1) = "/support/default.aspx"
	dataArray(1,2) = False
	dataArray(2,0) = "Create New Case"
	dataArray(2,1) = "/support/case/newcase.asp"
	dataArray(2,2) = False
	dataArray(3,0) = "Find Existing Case"
	dataArray(3,1) = "/support/search/default.asp"
	dataArray(3,2) = False
	If(Extranet) Then
		dataArray(4,0) = "CDW Support Chat"
		dataArray(4,1) = "/shop/eaccount/ChatMain.aspx?Pp=1"
		dataArray(4,2) = False
	end if
	dataArray(5,0) = "FTP Site"
	dataArray(5,1) = "ftp://ftp.cdw.com/pub/"
	dataArray(5,2) = False
	dataArray(6,0) = "Vendor Links"
	dataArray(6,1) = "/webcontent/inside/support/manufacturers.asp"
	dataArray(6,2) = False
	dataArray(7,0) = "Contact Support"
	dataArray(7,1) = "/webcontent/inside/support/contact.asp"
	dataArray(7,2) = False

	bArray = dataArray
end function

' Career links for CDWG.com
function Get_CareerLinks(bArray)
	dim dataArray(4,2)

	dataArray(0,0) = "CareerLinks"
	dataArray(0,1) = ""
	dataArray(0,2) = False
	dataArray(1,0) = "Available Positions"
	dataArray(1,1) = "/webcontent/insideG/employment/jobs.asp?d=CDWG"
	dataArray(1,2) = False
	dataArray(2,0) = "Benefits &amp; Extras"
	dataArray(2,1) = "/webcontent/insideG/career/benefitsextras.asp"
	dataArray(2,2) = False
	dataArray(3,0) = "Chicago Life"
	dataArray(3,1) = "/webcontent/insideG/career/chicagolife.asp"
	dataArray(3,2) = False
	dataArray(4,0) = "Directions"
	dataArray(4,1) = "/webcontent/insideG/career/directions.asp"
	dataArray(4,2) = False

	bArray = dataArray
end function

'********************************************************************************************************************
'*	UI Functions for the site listed below:																			*
'********************************************************************************************************************
'Draw the dividing cell to go between the menus on the main drop down navigation
sub DrawCellDivider
	Response.write(_
		"<td nowrap bgcolor='#444444'>" &_
			"<img src='" & GetImagePath("/global/pixels/none.gif") & "' height='24' width='1' alt='' />" &_
		"</td>" )
end sub

'Draw the dividing row to go between the rows of the sidebar menus
sub DrawRowDivider
	Response.write(_
		"<tr>"  &_
			"<td colspan='3'>" &_
				"<img width='1' height='3' src='" & GetImagePath("/global/pixels/none.gif") & "' alt='' />" &_
			"</td>"  &_
		"</tr>" )
End Sub

Function RenderBlueNuggetOpen(filename, altTag, height)
	Dim ReturnValue
		ReturnValue = (_
			"<table width='146' height='22' border=0' cellspacing='0' cellpadding='0' bordercolor='#CE0000'>" &_
				"<tr height='22'>" &_
					"<td align='left' valign='top' height='22' width='7'>" & _
						"<img src='" & GetImagePath("/global/parts/tabs/brown_left.gif") & "' alt=' ' />" & _
					"</td>" &_
					"<td bgcolor='#574128' height='22' valign='middle' width='132' background='" & GetImagePath("/global/parts/tabs/brown_mid.gif") & "'>" & _
						"<img src='" & GetImagePath("/macWarehouse/hubs/Hub_labels/" & fileName ) & "' alt='" & altTag & "' />" & _
					"</td>" &_
					"<td align='right' valign='top' height='22' width='7'>" & _
						"<img src='" & GetImagePath("/global/parts/tabs/brown_right.gif") & "' alt='' />" & _
					"</td>" &_
				"</tr>" & VbCrLf &_
			"</table>" & VbCrLf)

		ReturnValue = (ReturnValue & _
			"<table width='146' height='" & (height - 22) & "' border=0' cellspacing='0' cellpadding='0' bordercolor='#CE0000'>" &_
			"<tr height='" & (height - 28) & "'>" & _
				"<td width='1' bgcolor='#574128'>" & _
					"<img src='" & GetImagePath("/global/pixels/none.gif") & "' width='1' height='1' alt='' />" & _
				"</td>" & _
				"<td width='6'>" & _
					"<img src='" & GetImagePath("/global/pixels/none.gif") & "' width='6' height='1' alt='' />" & _
				"</td>" & _
				"<td valign='top'>" & _
					"<table border=0  cellpadding=0 cellspacing='0' height='100%'>" &_
						"<tr>" &_
							"<td valign='top' height='100%'>")
	RenderBlueNuggetOpen = ReturnValue
End Function

Function RenderBlueNuggetClose()
	Dim ReturnValue
		ReturnValue = (_
							"</td>" &_
						"</tr>" & VbCrLf & _
					"</table>" & VbCrLf & _
				"</td>" & _
				"<td width='6'>" & _
					"<img src='" & GetImagePath("/global/pixels/none.gif") & "' width='6' height='1' alt='' />" & _
				"</td>" & _
				"<td width='1' bgcolor='#574128'>" & _
					"<img src='" & GetImagePath("/global/pixels/none.gif") & "' width='1' height='1' alt='' />" & _
				"</td>" & _
			"</tr>" & VbCrLf & _
			"<tr height='6'>" & _
				"<td colspan='2' height='6' width='7' valign='bottom'>" & _
					"<img src='" & GetImagePath("/global/parts/corners/brown/btm_left.gif") & "' height='6' width='6'  alt='' />" & _
					"<img src='" & GetImagePath("/global/pixels/574128.gif") & "' height='1' width='1' alt='' />" & _
				"</td>" & _
				"<td height='6' valign='bottom'><img src='" & GetImagePath("/global/pixels/574128.gif") & "' width='100%' height='1' alt='' /></td>" & _
				"<td colspan='2' height='6' width='7' valign='bottom'>" & _
					"<img src='" & GetImagePath("/global/pixels/574128.gif") & "' height='1' width='1' alt='' />" & _
					"<img src='" & GetImagePath("/global/parts/corners/brown/btm_right.gif") & "' height='6' width='6' alt='' />" & _
				"</td>" & _
			"</tr>" & VbCrLf & _
		"</table>" & VbCrLf & _
		"<img src='" & GetImagePath("/global/pixels/none.gif") & "' width='1' height='7' alt='' /><br />")

	RenderBlueNuggetClose = ReturnValue
End Function

Function DrawMacSpecialLink(position, Callout, Text)
	Dim ReturnValue
		ReturnValue = (_
		"<tr>" & _
			"<td valign='top'>" & _
				"<img src='" & GetImagePath("/macWarehouse/gui/sidebar/red_arrow.gif") & "' vspace='3' border='0' />" & _
			"</td>" & _
			"<td valign='top'>" & _
				"<img src='" & GetImagePath("/global/pixels/none.gif") & "' width='5' height='5' alt='' /><br />" & _
			"</td>" & _
			"<td  valign='top' width='100%' class='NGbody'>" & _
				"<a href='javascript:goRotatorLink();' onMouseOver='rotateFeatureImage(" & position & "); rotateFeatureLink(" & position & ");' onMouseOut='rotateFeatureImage(1);' class='NGbody'>" & _
					"<span class='NGbodyBold'>" & Callout & "</span> - " & text & _
				"</a>" & _
			"</td>" & _
		"</tr>" & VbCrLf)

	DrawMacSpecialLink = ReturnValue
End Function

Function DrawInitialMacSpecial(EDC)
	Dim ImageSrc, ReturnValue

		ImageSrc	= GetProductImageLocationByCode(EDC, prProductImage80x)

	ReturnValue = (_
		"<tr>" & _
			"<td valign='top' colspan='3' class='NGbody' align='center'><div name='MacSpecial_image' id='MacSpecial_image'>" & _
				"<a name='rotator_target' href='javascript:goRotatorLink();'><img src='" & ImageSrc & "' name='MacSpecial_image' alt='Product Photo:  " & EDC & "' border='0' vspace='2'></a>" & _
			"</td>" & _
		"</tr>" & VbCrLf)

	DrawInitialMacSpecial = ReturnValue
End Function

Function DrawPageLink(Text, RedirectNumber, IsBold)
	Dim ReturnValue

	IsBold = CBool(IsBold)
	ReturnValue = (_
		"<tr>" & _
			"<td valign='top' width='4'>" & _
				"<img src='" & GetImagePath("/macWarehouse/gui/sidebar/red_arrow.gif") & "' vspace='3' border='0' />" & _
			"</td>" & _
			"<td valign='top' width='5'>" & _
				"<img src='" & GetImagePath("/global/pixels/none.gif") & "' width='5' height='5' alt='' /><br />" & _
			"</td>" & _
			"<td valign='top' width='100%' class='NGbody'>" & _
				"<a href='/r.aspx?n=" & RedirectNumber & "' class='NGbody'>")

				If IsBold = True Then ReturnValue = ReturnValue & ("<span style='font-weight: bold;'>") End If
					ReturnValue = ReturnValue & (text)
				If IsBold = True Then ReturnValue = ReturnValue & ("</span>") End If

	ReturnValue = ReturnValue & (_
				"</a>" & _
			"</td>" & _
		"</tr>" & VbCrLf)

	DrawPageLink = ReturnValue
End Function

'********************************************************************************************************************
'*	Count No of EDC's Querystring Values:																			*
'********************************************************************************************************************
Function CountQuerystringValues()
	Dim cnt
	cnt = 0
	FOR i=1 to Request.Querystring("edc").Count 
		cnt= cnt + 1
	NEXT
 CountQuerystringValues = cnt
End Function

Function RegularNuggetSpacingTable()
	Dim ReturnValue
		ReturnValue = (_
		"<table border='0' cellspacing='0' cellpadding='0' height='100%' width='132'>" & _
			"<tr>" & _
				"<td colspan='3'>" & _
					"<img width='1' height='6' src='" & GetImagePath("/global/pixels/none.gif") & "' alt='' />" & _
				"</td>" & _
			"</tr>" & VbCrLf)
	RegularNuggetSpacingTable = ReturnValue
End Function

Sub IncludeUrl(TargetUrl)
	 Dim ServerXMLHTTP
	 ServerXMLHTTP = Null
	 Set ServerXMLHTTP = CreateObject("Msxml2.ServerXMLHTTP.6.0")
	 Call ServerXMLHTTP.setTimeouts(10000, 10000, 40000, 40000)

	 ServerXMLHTTP.open "GET", TargetUrl, false, "", ""
	 Dim Headers
	 Headers = Split(Request.ServerVariables("ALL_RAW") , vbCr )
 	 For i  = 0 To UBound(Headers) - 1
		Dim Header
		Header = Split(Headers(i) , ": ")
		ServerXMLHttp.setRequestHeader Replace((CStr(Header(0))), vbLf, ""), Replace((CStr(Header(1))), vbLf, "")
	 Next
	 On Error Resume Next
	 ServerXMLHttp.setRequestHeader "Accept-Encoding", ""
	 On Error GoTo 0
	 
	Call ServerXMLHTTP.send()
	 
	 If IsEmpty(ServerXMLHTTP.responseBody)	Then
		Response.Redirect("/default.aspx")	
		Response.End()
	 Else	 
		Response.Write(Replace(ServerXMLHTTP.responseText, "http://", "https://"))
	 End If
End Sub

Sub DrawMenuItem(link, name)
	Response.write(_
		"<tr>"  &_
			"<td width='5' valign='middle' align='left'>" & _
				"<img width='5' height='1' src='" & GetImagePath("/global/pixels/none.gif") & "' alt='' />" & _
			"</td>"  &_
			"<td width='10' align='left' valign='top' class='MenuLink'>&#183;</td>"  &_
			"<td width='123' valign='top'>")
				If UserLoggedInSecure Then
					Response.write("<a href='https://" & glMainSiteName & link & "'" )
				Else
					Response.write("<a href='http://" & glMainSiteName & link & "'" )
				End If
	Response.write(_
				"CLASS='SidebarLink' title='" & name & "'>" & name & "</A><br />" &_
			"</td>"  &_
		"</tr>")

	Call DrawRowDivider()
End Sub


%>

