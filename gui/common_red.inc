<!--#INCLUDE VIRTUAL="/global/subs/errorhandling.inc"-->
<!--#INCLUDE VIRTUAL="/global/subs/MenuSubs.inc"-->
<!--#INCLUDE VIRTUAL="/global/subs/WebContentSubs.inc"-->
<!--#INCLUDE VIRTUAL="/webcontent/includes/WeightedRandomInclude.inc"-->
<!--#INCLUDE VIRTUAL="/webcontent/includes/WebSitePhoneNumbers.inc"-->
<%
'*********************************************************************************************************************
' global variables
	dim glpageTitle
	dim glpageDescription
	dim glpageImage
	dim glPrintable
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
	Dim glLabelPath, HeaderPromo, MWLeftSideBarInclude, FooterFile
	Dim glCompanyShortName
	Dim glRightSideBarFileName

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
	glPageSecure			= False
	glShowHeaderOnly 		= False
	glFlagShowMainHeader 	= True
	glCurrentSite			= GetThisSite()
	glCurrentSiteName		= GetSiteName(glCurrentSite)
	glCurrentURL 			= getThisPage()
	glRedirectURL 			= "/default.asp"
	glReferrerRequirements 	= glAnyReferrer
	glLogonRequirements 	= glAnyLogon
	glSecureRequirements 	= glAnySecure 'glRequireNonSecure
	glSupressNavigation		= false
	glDestination 			= null
	glPrintable				= False
	glCustomPage			= False
	glClosedLoop 			= False
	glTrueClosedLoop		= False
	glUseBodyOnLoadEvent	= False
	glCompanyShortName		= "CDW"
	glRightSideBarFileName	= null

	'if Trim(Request.Cookies(glSiteLocationCookie)) = glSiteLocationCDWCACookie Then
	If GetThisSite() = www_cdw_ca Then
		glCDWCanadaUser = true
		glCompanyShortName		= "CDW Canada"
	End If

	' Catch run time errors
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

	if glCDWCanadaUser = false Then
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
	End If

	If glMacWarehouseUser = True Then
		glLabelPath	= "/MacWarehouse/labels/"
		HeaderPromo	= DrawWebContentRandomIncludeFileWithWeight("/Includes/HeaderPromo/MacWarehouse/")
		MWLeftSideBarInclude = GetWebcontentIncludeFileString("/Includes/MacWarehouse/globalSB.asp")
		glRightSideBarFileName = "/Includes/MacWarehouse/Sidebars/default.html"
		FooterFile	= GetWebcontentIncludeFileString("/Includes/MacWarehouse/footer.htm")
	ElseIf glCDWCanadaUser = True Then
		glLabelPath	= "/cdw/labels/"
		FooterFile	= GetWebcontentIncludeFileString("/Includes/Canada/footer.htm")
		HeaderPromo	= DrawWebContentRandomIncludeFile("/Includes/HeaderPromo/cdw.ca/")
	Else
		glLabelPath	= "/cdw/labels/"
		HeaderPromo	= DrawWebContentRandomIncludeFileWithWeight("/Includes/HeaderPromo/cdw.com/red/")
		FooterFile	= GetWebcontentIncludeFileString("/Includes/CDW/footer.htm")
	End If

	If Cbool(Len(Trim(GetCookieValue(eaCookieCustomPage))) > 0 ) Then
		glCustomPage = True

		glCustomPageTitle = GetCustomPageName(GetCookieValue(eaCookieCustomPage))
			If IsNull(glCustomPageTitle) And IsEmpty(glCustomPageTitle) And Len(glCustomPageTitle) <> 0 Then
				glCustomPageTitle = "Custom Page"
			End If
	End If

	If Cbool(Len(Trim(GetCookieValue(eaCookieClosedLoop))) > 0 ) Then
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

	If glCDWCanadaUser Then
		glMainSiteName			= GetSiteName(www_cdw_ca)
		glMainSite				= www_cdw_ca
		glpageTitle 			= "CDW.ca"
	Else
		glMainSiteName			= GetSiteName(www_cdw_com)
		glMainSite				= www_cdw_com
		glpageTitle 			= "CDW.com"
	End If


	glHelpTopic				= Null
	'-------------------------------------------------------------------------
	' Web Reporting Cookies
	'	- TrackKey cookie... for the lifetime of the user
	'	- TrackSessionKey cookie... for this particular session
	If(Not(Len(Trim(GetCookieValue(eaCookieTrackKey))) > 0)) Then
        Call SetSingleCookie(eaCookieTrackKey, CreateGUID(), DateAdd("yyyy", 1, Date()), Empty )
	End If
	If(Not(Len(Trim(GetCookieValue(eaCookieTrackSessionKey))) > 0)) Then
        Call SetCookieValue (eaCookieTrackSessionKey, CreateGuid())
	End If
	'-------------------------------------------------------------------------

	'Set SessionKey cookie...
	If(Not(Len(Trim(GetCookieValue(eaCookieSessionKey))) > 0)) Then
		'Response.Cookies(eaCookieSessionKey).Path = "/"
		glSessionKey = CreateGUID()
		'Response.Cookies(eaCookieSessionKey) = glSessionKey
        Call SetCookieValue (eaCookieSessionKey, glSessionKey)
	Else
		glSessionKey = Trim(GetCookieValue(eaCookieSessionKey))
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

		' Auto logon...
		If(UserHasSavedCredentials()) Then
			If(Not(UserIsLoggedIn())) Then
				If Not(InStr(GetThisPage(),"404.aspx") > 0) Then
					Response.Redirect("https://" & RedirectSite & "/shop/eaccount/logontarget.asp?AutoLogOn=1&UserName=" & UrlEncode(Trim(GetCookieValue(eaCookieUserNameStore))) & "&Site=" & UrlEncode(GetThisSite()) & "&target=" & UrlEncode(GetThisPage()))
					Response.End()
				End If
			End If
		End If

		Call Fire_OnGetPageConstructor()
		Call Fire_OnGetPageSettings()

		Call ValidatePageContext(glCurrentURL, glRedirectURL)
		Call WriteHeadBodyTag()

		'******  Print out a warning if you are on either local, beta or staging...
		If ( glDevelopmentMode = glLocalMode or _
			glDevelopmentMode = glBetaMode or _
			glDevelopmentMode = glQAMode or _
			glDevelopmentMode = glStagingMode or _
			glDevelopmentMode = glPreviewMode _
		) Then
			Response.Write(_
				"<table border='0' cellspacing='0' cellpadding='0' width='100%' bgcolor='#CCCCCC' id='jMain' summary='local, beta or staging mode'>" &_
					"<tr>"  &_
						"<td nowrap height='25' align='center' valign='center' class='bodyHeadLarge'>" )
							select case glDevelopmentMode
								case glLocalMode
									Response.write("You are currently in LOCAL mode")
								case glBetaMode
									Response.write("You are currently in BETA mode")
								case glQAMode
									Response.write("You are currently in QA mode")
								case glStagingMode
									Response.write("You are currently in STAGING mode")
								case glPreviewMode
									Response.write(UCase("SITE PREVIEW:  preview.cdw.com"))
								case else
									Response.write("UNKNOWN MODE!!!")
							end select	
					Response.write(_
						"</td>"  &_
					"</tr>" &_
				"</table>")
		End If

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
			Call writeHeader(glheaderText)
		else
			if glFlagShowMainHeader then
				Call writeHeader(glheaderText)
			else
				'If glFlagSideHeader = True Then
				'	Call writeSideHeader()
				'Else
					Call writePetiteHeader()
				'End If
			end if

			If glTrueClosedLoop <> true Then
				Call Fire_OnGetSidebarDataLinks(TSidebarLinksArray)
				Call Fire_OnGetBottomDataLinks(BSidebarLinksArray)
				Call Fire_OnGetSidebarContent(sidebarContentArray)
			End If

			Response.write(_
							"<table border='0' cellspacing='0' cellpadding='0' bordercolor='orange' id='jMain' summary='Main table for page: sidebar and content'>" & _
								"<tr>" )

									'***call the sidebarData functions
									if glShowSidebar then
										Response.write(_
											"<td nowrap bgcolor='#F6F6F6' width='153' valign='top'>" &_
												"<table border='0' cellpadding='0' cellspacing='0' width='153'>" & _
													"<tr bgcolor='#F6F6F6'>" & _
														"<td bgcolor='#F6F6F6' nowrap height='100%'>" &_
															"<img src='" & GetImagePath("/global/pixels/none.gif") & "' height='1' width='5' alt='' />" &_
														"</td>"  & _
													 	"<td valign='top' width='140' height='100%' bgcolor='#F6F6F6'>"  & _
															"<img src='" & GetImagePath("/global/pixels/none.gif") & "' height='10' width='1' alt='' /><br />")

															If Cbool(Len(Trim(GetCookieValue(eaCookieCustomPage))) > 0 ) Then
																Call WriteCustomPageLink()
															End If

															If glTrueClosedLoop <> true Then
																if glShowResources then
																	Call WriteResourcesBar()
																end if

																Call WriteSidebarDataLinks(TSidebarLinksArray)
																Call WriteBottomDataLinks(BSidebarLinksArray)
																'***Call the sidebarContent functions
																Call writeSidebarContent(sidebarContentArray)
															End If

													Response.write(_
														"</td>" & _
														"<td nowrap bgcolor='#F6F6F6'>" &_
															"<img src='" & GetImagePath("/global/pixels/none.gif") & "' width='7' height='1' alt='' />" &_
														"</td>"  &_
													"</tr>" &_
													"<tr>" & _
														"<td colspan='3' bgcolor='#F6F6F6' align='left' style='padding-left: 10px;padding-right: 10px;''>" )
															If glMacWarehouseUser = false then
																Response.write(_
																"<img src='" & GetImagePath("/global/pixels/grey.gif") & "' height='1' width='100%' /><br />" & _
																"<img src='" & GetImagePath("/global/pixels/white.gif") & "' height='1' width='100%' /><br />" & _
																"<img src='" & GetImagePath("/global/pixels/none.gif") & "' height='7' width='100%' /><br />")
															End If

															'Added 10/2003 by Paul Haan
															'	Left sidebar for Mac Warehouse version of the site.
																If glMacWarehouseUser AND Not(glClosedLoop) then
																	'This uses the new HTML Include function from WebContentSubs.Inc
																	Response.write(MWLeftSideBarInclude)
																end If

															if (glMacWarehouseUser = False AND glCDWCanadaUser = False) then
																' Display the help Icon and link...
																If Not(isNull(glHelpTopic)) then
																	Response.write("<a href='javascript:PopupHelpWindow(""http://" & glMainSiteName & "/shop/help/default.asp?topic=" & HTMLEncode(glHelpTopic) & """)'>")
																Else
																	Response.write("<a href='javascript:PopupHelpWindow(""http://" & glMainSiteName & "/shop/help/default.asp"")'>")
																End If
																	Response.write(_
																			"<img src='" & GetImagePath("/cdw/labels/label_help.gif") & "' border='0' alt='Click for HELP  (popup window)' vspace='3' />" &_
																		"</a><br />")
															end if

															' Display the printable version icon and link...
															If glFlagShowMainHeader Then
																If glPrintable = False then
																	If Len(Request.form()) = 0 Then
																		Dim PrintableString
																		If Len(request.querystring()) > 0 Then
																			PrintableString	= "&printable=1"
																		Else
																			PrintableString	= "?printable=1"
																		End If

																		Response.write("<a href='" & HtmlEncode(glCurrentURL) & PrintableString & "' target='new' class='btslink'>" )

																		If glMacWarehouseUser then
																			Response.write("<img src='" & GetImagePath("/macWarehouse/gui/sidebar/printable.gif") & "' border='0' alt='Printable version  (popup window)' vspace='3' /></a><br />" )
																		Else
																			Response.write("<img src='" & GetImagePath("/cdw/labels/label_printpage.gif") & "' border='0' alt='Printable version  (popup window)' vspace='3' /></a><br />" )
																		End If
																	End If
																End If
															End If

													Response.Write (_
															"<br />" & _
														"</td>"  &_
													"</tr>" & _
												"</table>" & _
											"</td>")

									'End Sidebar Contents
										Response.write(_
											"<td bgcolor='#CCCCCC' nowrap>" &_
												"<img src='" & GetImagePath("/global/pixels/none.gif") & "' width='1' height='1' alt='' />" &_
											"</td>" )
											if glEnableLeftSpacing then
												'Change the spacing for the left side for Mac Warehouse site
												If glMacWarehouseUser  Then
													Response.write(_
													"<td>" &_
														"<img src='" & GetImagePath("/global/pixels/none.gif") & "' width='12' height='1' alt='' />" &_
													"</td>" )
												Else
													Response.write(_
													"<td>" &_
														"<img src='" & GetImagePath("/global/pixels/none.gif") & "' width='7' height='1' alt='' />" &_
													"</td>" )
												End IF
											end if
										end if

										'Changed the width of the cell if you are on Mac Warehouse, accounts for the right sidebar
										if glShowRightSideBar AND glMacWarehouseUser  Then
											Response.Write 		"<td valign='top'>"
										Else
											Response.Write 		"<td valign='top' width='100%'>"
										End If

										'Begin Datapane
										if glEnableTopSpacing then
											Response.write("<img src='" & GetImagePath("/global/pixels/none.gif") & "' height='10' width='1' alt='' /><br />" )
										end if
												if glShowTitleBar then
													Call WritePageTitleBar()
												end if

											Call DrawPageErrors()		' Write the new error handling block...
											Call Fire_OnGetMainBodyContent()
											Call writeFooter

				Response.Write			"</td>"
											'Added 10/2003 by Paul Haan
											'	Right sidebar for mac Warehouse version of the site
											'	Add spacing cell before the right sidebar.
											if glMacWarehouseUser and glShowRightSideBar then
												Response.write(_
												"<td valign='top' width='13'><img src='" & GetImagePath("/global/pixels/none.gif") & "' height='10' width='13' alt='' /></td>" & _
												"<td valign='top'>" & _
													"<img src='" & GetImagePath("/global/pixels/none.gif") & "' height='10' width='1' alt='' /><br />")

														'Added 3/24/2004 by Paul Haan
														'Changed the right sidebar from a straight include file to use the WebContentInclude functions.
														Dim RightSidebarContent
															RightSidebarContent	= GetWebcontentIncludeFileString(glRightSideBarFileName)

															Response.write(vbCrLf & RightSidebarContent & VbCrLf)

												Response.write("</td>")
											End If

											if glEnableLeftSpacing then
												Response.Write(_
													"<td valign='top'>" &_
														"<img src='" & GetImagePath("/global/pixels/none.gif") & "' width='10' height='1' alt='' />" &_
													"</td>" )
											end if
				Response.Write(_
									"</tr>" & _
									"<tr>")

								'Added 10/2003 by Paul Haan
								'	Extra row in the main table to put a bottom border on the site.
										Response.write(_
										"<td colspan='2' bgcolor='#CCCCCC' height='1'>" & _
											"<img src='" & GetImagePath("/global/pixels/none.gif") & "' alt='' height='1' width='100%'><br />" &_
										"</td>")

				Response.write(_
								"</tr>" & _
							"</table>" )
				'Response.write(_
				'		"<!--/td>" )

						'Added 10/2003 by Paul Haan
						'	Extra cell in the outer table to put a right border on the site.
						'If glMacWarehouseUser then
						'	Response.write(_
						'		"<td width='1' bgcolor='#CCCCCC' valign='top'>"  & _
						'			"<img src='" & GetImagePath("/global/pixels/none.gif") & "' alt='grey' height='21' width='1'><br />" &_
						'			"<img src='" & GetImagePath("/global/pixels/white.gif") & "' alt='red' height='66' width='1'><br />" &_
						'			"<img src='" & GetImagePath("/global/pixels/999999.gif") & "' alt='grey' height='24' width='1'><br />" &_
						'			"<img src='" & GetImagePath("/global/pixels/grey.gif") & "' alt='grey' height='1' width='1'><br />" )
						'Else
							'Changed on 10/28/03 to make the border transparent until a final decision is made...
						'	Response.write(_
						'		"<td width='1' bgcolor='#FFFFFF' valign='top'>"  & _
						'			"<img src='" & GetImagePath("/global/pixels/none.gif") & "' alt='none' height='100%' width='1'><br />" )
						'End If

				'		Response.write(_
				'		"</td>" & _
				'		"<td width='100%'>" & _
				'			"<img src='" & GetImagePath("/global/pixels/none.gif") & "' alt='' height='1' width='100%'>" & _
				'		"</td>" & _
				'	"</tr>" & _
				'"</table>" )

				' Have to put the floating divs at the bottom for IE5 on Mac OS 9 menus. >:(
				' Don't put this code in any tables, or wrap it with anything else. 
				' It must appear directly in front of the </body> tag...
				If GetThisSite() <> www_cdwg_com Then
					If glMacWarehouseUser = True Then
						MenuSubs_GenerateSubMenus MenuSubs_CdwMacMenu
					ElseIf GetThisSite() = www_cdw_ca Then
						MenuSubs_GenerateSubMenus MenuSubs_CdwCanadaMenu
					Else
						MenuSubs_GenerateSubMenus MenuSubs_CdwMenu
					End If
				End If

				' A div to serve as an expansion for IE5 on Mac OS 9, so that mouse
				' events are routed properly for the menus.
				Response.Write("<div id='blankDiv'></div>")

				'End BodyContent
				Response.write(_
					"</body>" & vbCrlf & _
					"</html>" & vbCrlf)
			Call Fire_OnGetPageDestructor()
		end if
	end if
	end sub	

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
'*		Lin_NS  == HTTP_USER_AGENT = Mozilla/5.0 (Macintosh; U; PPC Mac OS X; en-us) AppleWebKit/85 (KHTML, like Gecko) Safari/85	*
'*																																	*
'************************************************************************************************************************************

	dim CSSFile
	CSSFile = "Mac_IE"

		If InStr(Request.ServerVariables("HTTP_USER_AGENT"), "Mac") > 0 then
			If InStr(Request.ServerVariables("HTTP_USER_AGENT"), "MSIE") > 0 then
				CSSFile = "Mac_IE"
			ElseIF InStr(lcase(Request.ServerVariables("HTTP_USER_AGENT")), "safari") > 0 Then
				CSSFile = "Safari"
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
end sub

'*********************************************************************************************************************
	'Writes out the header and title.
Sub WriteHeadBodyTag()
	dim mac

	Response.Write(_
		"<!DOCTYPE HTML PUBLIC '-//W3C//DTD HTML 4.0 Transitional//EN'>"  &_
		"<html lang='EN' Content-Type: text/html; charset=ISO-8859-1>"  & _
		"<head>" & _
			"<script language='javascript' type='text/javascript' src='/global/gui/javascript/glwindow.js'></script>" & _
			"<script language='javascript' type='text/javascript' src='/global/gui/javascript/global.js'></script>")

			Response.write("<script language='javascript' type='text/javascript'>" )
		'Write out the client-side BODY event functions
		Call writeBodyEvents()

	Response.Write("</script>" )
		Call Fire_OnGetClientScripts()

	Response.write("<title>" & glpageTitle & "</title>")

		Call WriteOutCSSLink()

	if glUseBodyOnLoadEvent = True then
		'MUST HAVE bodyLoad() javascript method on client
		response.write(_
			"</head>"  &_
			"<body onLoad='bodyLoad()' marginwidth='0' marginheight='0' topmargin='0' leftmargin='0' bgcolor='#FFFFFF' text='#000000' link='#000000' vlink='#000000' alink='#000000'>"  &_
			"<a name='Top'></a>"   )
	else
		response.write(_
			"</head>" &_
			"<body marginwidth='0' marginheight='0' topmargin='0' leftmargin='0' bgcolor='#FFFFFF' text='#000000' link='#000000' vlink='#000000' alink='#000000'>"  &_
			"<a name='Top'></a>"   )
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
	Response.write(_
		"<table width='85%' cellpadding='5' cellspacing='5' border='0' summary='table for printable page header'>" &_
			"<tr>" &_
				"<td align='left' rowspan='2'>")

				If glMacWarehouseUser = True Then
					Response.write("<img src='" & GetImagePath("/global/logos/macWareHouse/logo_macwarehouse.gif") & "' border='0' alt='CDW  |  MacWarehouse' />")
				ElseIf glCDWCanadaUser = True Then
					Response.write("<img src='" & GetImagePath("/global/logos/Canada/printable.gif") & "' border='0' alt='CDW Canada' />")
				Else
					Response.write("<img src='" & GetImagePath("/global/logos/cdw/cdw_printablepage_logo.gif") & "' border='0' alt='CDW Logo' />")
				End If

	Response.write(_
				"</td>" & _
				"<td align='right' valign='middle' nowrap><b>" &_
					"<font size=3 face='verdana,helvetica,arial' color='#000000'>")

						If Len(CurPhoneNumber) > 0 Then
							Response.write(HTMLEncode(CurPhoneNumber))
						Else
							Response.write(OnGetPhoneNumber())
						End If

				Response.write(_
					"</font></b>" & _
				"</td>" &_
			"</tr>" &_
		"</table>"  )
	Response.write (_
		"<table border='0' cellpaddding=0 cellspacing='0' width='90%' summary='Table for body of printable version'>" &_
			"<tr>" &_
				"<td valign='top' >")
					if glEnableTopSpacing then
						Response.write("<img src='" & GetImagePath("/global/pixels/none.gif") & "' height='15' width='1' alt='' /><br />")
					end if
					Call Fire_OnGetMainBodyContent()
	Response.write(_
				"</td>" &_
			"</tr>" &_
		"</table>"     &_
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
	dim bgcolor, topnavbg

	If glMacWareHouseUser = True Then
		bgcolor		= "FFFFFF"
		topnavbg	= "D2E8FF"
	ElseIf glCDWCanadaUser = True Then
		bgcolor		= "FFFFFF"
		topnavbg	= "FFFFFF"
	Else
		bgcolor		= "CC0000"
		topnavbg	= "FFFFFF"
	End If

		If glTrueClosedLoop Then
				response.write(_
				"<table width='100%' cellpadding='0' cellspacing='0' border='0'  bgcolor='" & bgcolor & "' bordercolor='#FF0000' summary='Main header table'>" &_
					"<tr>" & _
						"<td rowspan='2' nowrap width='335'>" &_
							"<table cellpadding='0' cellspacing='0' border='0'  bgcolor='" & bgcolor & "' summary='Table for header logos'>" &_
								"<tr>" &_
									"<td valign='top' align='left' bgcolor='" & bgcolor & "'>" & _
										"<a href='/shop/custompages/default.aspx?CustomPageKey=" & HTMLEncode(GetCookieValue(eaCookieCustomPage)) & "'>" )
											If glMacWareHouseUser = True Then
												Response.write("<img src='" & GetImagePath("/global/logos/macWareHouse/logo_macwarehouse.gif") & "' border='0' alt='CDW  |  MacWarehouse' />" )
											ElseIf glCDWCanadaUser = True Then
												Response.write("<img src='" & GetImagePath("/global/logos/Canada/CDW_Canada.gif") & "' border='0' alt='CDW Canada' />")
											Else
												Response.write("<img src='" & GetImagePath("/global/logo_red.gif") & "' border='0' alt='CDW - The Right Technology. Right Away.&trade;' />" )
											End If

										Response.write(_
										"</a>" &_
									"</td>" &_
								"</tr>" &_
							"</table>" &_
						"</td>" &_
					"</tr>" &_
				"</table>" )
				If glMacWareHouseUser = True OR (glCDWCanadaUser = True) Then
					Response.write("<img src='" & GetImagePath("/global/pixels/black.gif") & "' border='0' alt='' height='1' width='100%' /><br />")
				End If

				Response.write(_
					"<table border='0' cellpadding='0' cellspacing='0' width='100%'>" & _
						"<tr>" & _
							"<td width='100%' height='24' nowrap align='right' bgcolor='#AAAAAA' class='num'>" )

								If Len(CurPhoneNumber) > 0 Then
									Response.write(HTMLEncode(CurPhoneNumber))
								Else
									Response.write(OnGetPhoneNumber())
								End If

						Response.write(_
								 "&nbsp;" &_
							"</td>"   &_
						"</tr>"   &_
					"</table>" &_
					"<img width='100%' height=1 src='" & GetImagePath("/global/pixels/black.gif") & "' alt='' />")
		Else
			Response.write(_
				"<table width='100%' cellpadding='0' cellspacing='0' border='0' bgcolor='#" & bgcolor & "' bordercolor='#FF0000' summary='Main header table'>"  &_
					"<tr>"  &_
						"<td rowspan='2' nowrap width='335'>" &_
							"<table cellpadding='0' cellspacing='0' border='0' bgcolor='#" & bgcolor & "' bordercolor='#0000FF' summary='Table for header logos'>"  &_
								"<tr>" )
						If glMacWareHouseUser = True Then		'MacWarehouse header: Logo and Promo spot
							Response.write(_
								"<td valign='top' align='left' bgcolor='#" & bgcolor & "'>" &_
									"<a href='http://" & glMainSiteName & "/default.asp' " & targettop & ">" &_
										"<img src='" & GetImagePath("/global/logos/macWareHouse/logo_macwarehouse.gif") & "' border='0' vspace='0' alt='CDW  |  MacWarehouse' />" &_
									"</A>" &_
								"</td>" &_
								"<td valign='top' align='left' bgcolor='#" & bgcolor & "'>&nbsp;</td>" &_
								"<td valign='top' align='left' bgcolor='#" & bgcolor & "' rowspan='2'>" & HeaderPromo & "</td>" )
						ElseIF glCDWCanadaUser = True Then		'CDW Canada header: Logo and Promo spot
							Response.write(_
								"<td valign='top' align='left' bgcolor='#" & bgcolor & "'>" &_
									"<a href='http://" & glMainSiteName & "/default.asp' " & targettop & ">" &_
										"<img src='" & GetImagePath("/global/logos/Canada/CDW_Canada.gif") & "' border='0' vspace='0' alt='CDW Canada' />" &_
									"</A>" &_
								"</td>" &_
								"<td valign='top' align='left' bgcolor='#" & bgcolor & "'>&nbsp;</td>" &_
								"<td valign='top' align='left' bgcolor='#" & bgcolor & "' rowspan='2'>" & HeaderPromo & "</td>" )
						ElseIf glDevelopmentMode = glPreviewMode Then		'Preview header: Logo and Promo spot
							Response.write(_
								"<td valign='top' align='left'>" &_
									"<a href='http://" & glMainSiteName & "/default.asp' " & targettop & ">" &_
										"<img src='" & GetImagePath("/global/logo_red.gif") & "' border='0' alt='CDW - The Right Technology. Right Away.&trade;' />" &_
									"</A>" &_
								"</td>" &_
								"<td valign='top' align='left' bgcolor='#" & bgcolor & "'>&nbsp;</td>" &_
								"<td valign='top' align='left' rowspan='2'>" &_
									"<a href='http://" & glMainSiteName & "/r.asp?n=33227' " & targettop & ">" &_
										"<img src='" & GetImagePath("/cdw/gui/topimagePreview.gif") & "' border='0' alt='Please give us your feedback' />" &_
									"</a>" &_
								"</td>" )
						ElseIf Cbool(Len(Trim(GetCookieValue(eaCookieCustomPage))) > 0 ) Then
							If glClosedLoop Then		'Custom Page (closed loop) header: Logo and Promo spot
								Response.write(_
								"<td valign='top' align='left'>" &_
									"<a href='http://" & glMainSiteName & "/shop/custompages/default.aspx?CustomPageKey=" & HTMLEncode(GetCookieValue(eaCookieCustomPage)) & "' " & targettop & ">" &_
										"<img src='" & GetImagePath("/global/logo_red.gif") & "' border='0' alt='CDW - The Right Technology. Right Away.&trade;' />" &_
									"</A>" &_
								"</td>" &_
								"<td valign='top' align='left' bgcolor='#" & bgcolor & "'>&nbsp;</td>" &_
								"<td valign='top' align='left' rowspan='2'>" &_
									"<img src='" & GetImagePath("/cdw/gui/topimage_Blank.gif") & "' border='0' alt='' />" &_
								"</td>" )
							Else		'Custom Page header: Logo and Promo spot
								Response.write(_
								"<td valign='top' align='left'>" &_
									"<a href='http://" & glMainSiteName & "/default.asp' " & targettop & ">" &_
										"<img src='" & GetImagePath("/global/logo_red.gif") & "' border='0' alt='CDW - The Right Technology. Right Away.&trade;' />" &_
									"</A>" &_
								"</td>" &_
								"<td valign='top' align='left' bgcolor='#" & bgcolor & "'>&nbsp;</td>" &_
								"<td valign='top' align='left' rowspan='2'>" &_
									"<img src='" & GetImagePath("/cdw/gui/topimage_Blank.gif") & "' border='0' alt='' />" &_
								"</td>" )
							End If
						Else		'Regular CDW header: Logo and Promo spot
							Response.write(_
							"<td valign='top' align='left'>" &_
								"<a href='http://" & glMainSiteName & "/default.asp' " & targettop & ">" &_
									"<img src='" & GetImagePath("/global/logo_red.gif") & "' border='0' alt='CDW - The Right Technology. Right Away.&trade;' />" &_
								"</A>" &_
							"</td>" &_
							"<td valign='top' align='left' bgcolor='#" & bgcolor & "'>&nbsp;</td>" &_
							"<td valign='top' align='left' rowspan='2'>" & HeaderPromo & "</td>" )
						End If

			Response.write("</tr>")
			If (glCDWCanadaUser = False) AND (glClosedLoop = False) AND (glTrueClosedLoop = False) Then
				Response.write(_
					"<tr>" &_
						"<td colspan='2'>")

						'This is the new section for selecting the tabs between CDW and MacWarehouse
						If glMacWarehouseUser = true Then
							Response.write("<a href='/ChangeVM.asp?svmc=" & eaVMCDW & "' " & targettop & "><img src='" & GetImagePath("/cdw/gui/tab_cdw_inactive.gif") & "'  border='0' alt='CDW' /></a>")
							Response.write("<a href='/ChangeVM.asp?svmc=" & eaVMMacWarehouse & "' " & targettop & "><img src='" & GetImagePath("/cdw/gui/tab_mw_active.gif") & "' border='0' alt='MacWarehouse' /></a>")
						Else
							Response.write("<a href='/ChangeVM.asp?svmc=" & eaVMCDW & "' " & targettop & "><img src='" & GetImagePath("/cdw/gui/tab_cdw_active_red.gif") & "' border='0' alt='CDW' /></a>")
							Response.write("<a href='/ChangeVM.asp?svmc=" & eaVMMacWarehouse & "' " & targettop & "><img src='" & GetImagePath("/cdw/gui/tab_mw_inactive_red.gif") & "' border='0' alt='MacWarehouse' /></a>")
						End If

				Response.write(_
						"</td>" & _
					"</tr>")
			End If

			Response.write("</table>" )

response.write(_
		"</td>"  &_
		"<td width='100%' align='right' height='21' valign='top'>"  &_
			"<table border='0' bordercolor='#0000FF' cellpadding='0' cellspacing='0' bgcolor='#" & topnavbg & "' width='100%' summary='Table for top customer links'>"  &_
				"<tr>"  &_
					"<td height='21' width='100%' nowrap valign='middle' align='left' bgcolor='#" & topnavbg & "'>" &_
						"<img src='" & GetImagePath("/global/pixels/none.gif") & "' height='1' width='100%' alt='' />" &_
					"</td>"  &_
					"<td height='21' nowrap valign='middle' align='left' class='MenuLink'>|&nbsp;" )
						If Cbool(Len(Trim(GetCookieValue(eaCookieCustomPage))) > 0 ) Then
							If glClosedLoop Then
								Response.write("<a href='http://" & glMainSiteName & "/shop/custompages/default.aspx?CustomPageKey=" & HTMLEncode(GetCookieValue(eaCookieCustomPage)) & "' class='MenuLink' " & targettop & " title='Home'>Home</a>")
							Else
								Response.write("<a href='http://" & glMainSiteName & "/default.asp' class='MenuLink' " & targettop & " title='Home'>Home</a>")
							End If
						Else
							Response.write("<a href='http://" & glMainSiteName & "/default.asp' class='MenuLink' " & targettop & " title='Home'>Home</a>")
						End If

					dim AboutName, AboutPage
					If glCDWCanadaUser Then
						AboutName	= "About Us"
						AboutPage	= "/webcontent/insideCA/default.asp"
					Else
						AboutName	= "About CDW"
						AboutPage	= "/webcontent/inside/default.asp"
					End If

					Response.write(_
						"</td>"  &_
					"<td height='21' nowrap valign='middle' align='left' class='MenuLink'>&nbsp;|&nbsp;</td>"  &_
					"<td height='21' nowrap valign='middle' align='left'>" &_
						"<a href='http://" & glMainSiteName & AboutPage & "' class='MenuLink' " & targettop & " title='" & AboutName & "'>" & AboutName & "</a>" &_
					"</td>" )

				If glMacWarehouseUser = false Then
				Response.write(_
					"<td height='21' nowrap valign='middle' align='left' class='MenuLink'>&nbsp;|&nbsp;</td>"  &_
					"<td height='21' nowrap valign='middle' align='left'>" &_
						"<a href='http://" & glMainSiteName & "/webcontent/inside/service/default.asp' class='MenuLink' " & targettop & " title='Customer Support'>Customer Support</a>" &_
					"</td>" )
				End If

				Response.write(_
					"<td height='21' nowrap valign='middle' align='left' class='MenuLink'>&nbsp;|&nbsp;</td>"  &_
					"<td height='21' nowrap valign='middle' align='left'>" &_
						"<a href='http://" & glMainSiteName & glShoppingCartURL & "' class='MenuLink' " & targettop & " title='View Cart'>View Cart</a>" &_
					"</td>"  &_
					"<td height='21' nowrap valign='middle' align='left' class='MenuLink'>&nbsp;|&nbsp;</td>" )
					
				If glMacWarehouseUser = true then
					Response.write(_
					"<td height='21' nowrap valign='middle' align='left'>" &_
						"<a href='http://" & glMainSiteName & HtmlEncode(glCurrentURL) )
							If Len(request.querystring()) > 0 Then
								Response.write("&printable=1")
							Else
								Response.write("?printable=1")
							End If
					Response.write(_
						 "' target='new' class='MenuLink' title='Printable Page (popup window)'>Printable Page</a>" &_
					"</td>"  &_
					"<td height='21' nowrap valign='middle' align='left' class='MenuLink'>&nbsp;|&nbsp;</td>" )
				End If

					If(UserIsLoggedIn()) Then
						Response.Write(_
							"<td nowrap height='21' valign='middle' align='left'>" &_
								"<a href='http://" & glMainSiteName & "/shop/eaccount/logoff.asp' class='MenuLink' " & targettop & " title='Log Off'>Log Off</a>" &_
							"</td>"  )
					Else
						Response.Write("<td nowrap height='21' valign='middle' align='left'>")

						If(Not(glLogonLinkExempt)) Then
							Response.Write("<a href='https://" & glMainSiteName & glBaseLogonURL & "?site=" & HTMLEncode(glCurrentSite) & "&target=" & HTMLEncode(glCurrentURL) &"' class='MenuLink' style='FONT-WEIGHT:800;COLOR:#FF0000' " & targettop & ">")
						End If

						If glMacWarehouseUser = true then
							Response.Write("<img src='" & GetImagePath("/global/icon_logon_mw.gif") & "' align='left' border='0' alt='Logon' />")
						Else
							Response.Write("<img src='" & GetImagePath("/global/logon_orange.gif") & "' align='left' border='0' alt='Logon' />")
						End If

						If(Not(glLogonLinkExempt)) Then 
							Response.Write("</a>") 
						End If

						Response.Write(_
							"</td>"  &_
							"<td nowrap height=21 valign='middle' align='left' class='bts'>" &_
								"&nbsp;")

						If(Not(glLogonLinkExempt)) Then
							Response.Write("<a href='https://" & glMainSiteName & glBaseLogonURL & "?site=" & HTMLEncode(glCurrentSite) & "&target=" & HTMLEncode(glCurrentURL) &"' class='MenuLink' ")
							If glMacWarehouseUser = true then
								Response.Write("style='FONT-WEIGHT:800;COLOR:#000000'")
							Else
								Response.Write("style='FONT-WEIGHT:800;COLOR:#FF9933'")
							End If
							Response.write( targettop & " title='Log On'>")
						Else
							If glMacWarehouseUser = true then
								Response.Write("<span class='MenuLink' style='font=weight:800;'>")
							Else
								Response.Write("<span class='MenuLink' style='font=weight:800;COLOR:#FF0000'>")
							End If
						End If

						Response.Write("Log On")

						If(Not(glLogonLinkExempt)) Then 
							Response.Write("</a>")
						Else
							Response.Write("</span>")
						End If

						Response.Write("</td>" )
					end if
			Response.write("<td nowrap>&nbsp;</td>" )


			Response.write(_
				"</tr>"  &_
			"</table>"   &_
		"</td>"  &_
	"</tr>" )

'Build querystring for contracts for closed loop sites, and hidden input variables
Dim contractQryStr
Dim contractHiddenVars

If glClosedLoop Then
	contractQryStr = "?"
	contractHiddenVars = ""

	'for loop to run through contract when site is in closed loop mode
	'Session contracts
	If(IsArray(CurSessionContracts)) Then
		For z = LBOUND(CurSessionContracts,2) to UBOUND(CurSessionContracts,2)
			CurContractID = CurSessionContracts(eaSessionValue,z)
			contractHiddenVars = contractHiddenVars & "<input type='hidden' name='con' value='" & HTMLEncode(Trim(CurContractID)) & "' />"
			contractQryStr = contractQryStr & "con=" & HTMLEncode(Trim(CurContractID)) & "&"
		Next
	End If	
End If
	dim textclass
If (glMacWarehouseUser = True) OR (glCDWCanadaUser = True) Then
	textClass = "sst"
Else
	textClass = "sstWhite"
End If
Response.write(_
	"<tr>"  &_
		"<td width='100%' valign='middle' align='right' height='44' bgcolor='#" & bgcolor & "'>"  &_
			"<table border='0' cellpadding='0' cellspacing='0' summary='Table for smart search area'>"  &_
				"<tr>"  &_
				"<form action='http://" & glMainSiteName & "/shop/search/" & GetSearchPage() & "' " & targettop & " method='get'>" &_
					"<td valign='top' align='center'>" &_
						"<input type='text' id='key' NAME='key' size='15' maxlength='25' class='searchbox' /><br />" &_
						"<img width='100%' height='3' src='" & GetImagePath("/global/pixels/none.gif") & "' alt='' /><br />" & _
						"<table border='0' cellspacing='0' cellpadding='0' width='100%' summary='Search filter table: Mac, PC, All'>" & _
							"<tr>" & _
				  				"<td><input id='macsearch' name='platform' type='radio' value='mac' " & radiostyle )
									If glMacWarehouseUser = true then Response.write(" checked ") End If
						Response.write(_
								"></td>" & _
				  				"<td class='" & textClass & "'><label for='macsearch'>Mac&nbsp;</label></td>" & _
				  				"<td><input id='pcsearch' type='radio' name='platform' value='pc' " & radiostyle & "></td>" & _
				  				"<td class='" & textClass & "'><label for='pcsearch'>PC&nbsp;</td>" & _
				  				"<td><input id='allsearch' type='radio'  name='platform' value='all' " & radiostyle)
									If glMacWarehouseUser = false then Response.write(" checked ") End If
						Response.write(_
								"></td>" & _
				  				"<td class='" & textClass & "'><label for='allsearch'>All</label></td>" & _
							"</tr>" & _
			  			"</table>" & _
					"</td>"  &_
					"<td valign='middle'>" &_
						"<img width='7' height='1' src='" & GetImagePath("/global/pixels/none.gif") & "' alt='' />" )

					If glClosedLoop Then
						Response.Write(contractHiddenVars)
					End If

				Response.write(_
					"</td>"  &_
					"<td valign='top'>" &_
						"<input type='image' src='")

						If glMacWarehouseUser = true then
							Response.write(GetImagePath("/global/buttons/btn_search.gif") & "' ")
						Else
							Response.write(GetImagePath("/global/buttons/gy_search.gif") & "' vspace='2'")
						End If

				Response.write(_
							" border='0' alt='Search'>" &_
					"</td>"  &_
					"<td valign='top' height=44 class='" & textClass & "'>")

					If glClosedLoop Then
						Response.Write("<a href='http://" & glMainSiteName & "/shop/search/default.asp" & contractQryStr & "' " & targettop & " class='" & textClass & "' title='Smart Search'>")
					Else
						Response.Write("<a href='http://" & glMainSiteName & "/shop/search/default.asp' " & targettop & " class='" & textClass & "' title='Smart Search'>")
					End If

					Dim radiostyle, ImagePath
					If (glMacWareHouseUser = True) OR (glCDWCanadaUser = True) Then 
						radiostyle	= "class='MWradio'"
						ImagePath	= "/MacWarehouse"
					Else
						radiostyle	= "class='CDWradio'"
						ImagePath	= "/cdw"
					End If

						Response.write(_
							"<img src='" & GetImagePath(ImagePath & "/gui/SmartSearch_red.gif") & "' hspace='7' border='0' alt='SmartSearch&trade;'>" &_
						"</a></label>" &_
					"</td>"  &_
				"</form>" &_
				"</tr>"  &_
			"</table>"   &_
		"</td>"  &_
	"</tr>"  &_
"</table>"  )

		if glCDWCanadaUser = True then
			Response.write("<img src='" & GetImagePath("/global/pixels/black.gif") & "' height='1' width='100%' />")
		End If
		Call WriteCategoryMenu()
		if glMacWarehouseUser = false then
			Response.write("<img src='" & GetImagePath("/global/pixels/black.gif") & "' height='1' width='100%' />")
		End If
	End If
end sub
'*********************************************************************************************************************
'This will write the small header if the main header is not needed.
sub writePetiteHeader()
	'Start Petite Header Code
	dim bgcolor, Logopath, AltText

	if glMacWarehouseUser = True Then
		bgcolor		= "#FFFFFF"
		Logopath	= "macwarehouse"
		AltText		= "CDW | MacWarehouse"
	ElseIf glCDWCanadaUser = True Then
		bgcolor		= "#FFFFFF"
		Logopath	= "canada"
		AltText		= "CDW Canada"
	Else
		bgcolor		= "#000000"
		Logopath	= "cdw"
		AltText		= "CDW - The Right Technology. Right Away."
	End If


	Response.write(_
		"<table width='100%' cellpadding='0' cellspacing='0' border='0' bgcolor='" & bgcolor &"' summary='petite header table'>" &_
			"<tr>"  &_
				"<td width='100%' nowrap>" )

					If glCustomPage <> true Then
						Response.write ( "<a href='http://" & glMainSiteName & "/default.asp'>" )
					End If

					Response.write( "<img width='290' height='42' src='" & GetImagePath("/global/logos/" & Logopath & "/petiteheader.gif") & "' border='0' alt='" & AltText & "' />" )

					If glCustomPage <> true Then
						Response.write ( "</a>" )
					End If 

	Response.write (_
				"</td>"  &_
			"</tr>"  &_
		"</table>")

		if glMacWarehouseUser = True OR (glCDWCanadaUser = True)Then
			Response.write("<img src=" & GetImagePath("/global/pixels/black.gif") & " height='1' width='100%' />")
		End If
	'End Petite Header Code
end sub
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
 	  				"<td width='153' valign='top'>" & _
						"<table width='100%' border='0' cellspacing='0' cellpadding='0'>" & _
        					"<tr>" )

						If glMacWarehouseUser = True Then
							If glClosedLoop = true OR glTrueClosedLoop = True Then
								Response.write(_
									"<td width='47' valign='top'><img src='" & GetImagePath("/macwarehouse/gui/bevelbar_left.gif") & "' width='47' height='24'></td>" & _
          							"<td valign='top' bgcolor='#DFDFDF'><img src='" & GetImagePath("/macwarehouse/gui/bevelbar_left.gif") & "' width='106' height='24'></td>" )
							Else
								Response.write(_
									"<td width='47' valign='top'><img src='" & GetImagePath("/macwarehouse/gui/bevelbar_left.gif") & "' width='47' height='24'></td>" & _
	          						"<td valign='top' bgcolor='#DFDFDF'><img src='" & GetImagePath("/macwarehouse/gui/leftNav_bg_light.gif") & "' width='106' height='24'></td>" )
							End If
						Else
							Response.write(_
								"<td width='153' valign='top'><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='153' height='24'></td>" )
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
			"<td width='100%' nowrap align='right' bgcolor='#AAAAAA' class='num' ")

				If glMacWarehouseUser = true Then
					Response.write(" style='color:#666666;font-weight:800' background='" &  GetImagePath("/MacWarehouse/gui/bevelbar_right.gif") & "'")
				End If

		Response.write(_
			">" )

				If Len(CurPhoneNumber) > 0 Then
					Response.write(HTMLEncode(CurPhoneNumber))
				Else
					Response.write(OnGetPhoneNumber())
				End If
		Response.write(_
				 "&nbsp;" &_
			"</td>"   &_
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

								if TsideArr(ts,2) then
									Response.Write ("<a href='javascript:NewPopupWindow(""" & HTMLEncode(TsideArr(ts,1)) & """)' CLASS='SidebarLink' title='" & TsideArr(ts,0) & "  (Popup Window)'>")
								else
									Response.Write ("<a href='" & HTMLEncode(TsideArr(ts,1)) & "' CLASS='SidebarLink' title='" & TsideArr(ts,0) & "'>")
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
							"</td>"   &_
							"<td width=10 align='left' valign='top' class='SidebarLink'>" &_
								"&#183;</td>"   &_
							"<td width=125 valign='top'>")

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
							"<td width='140' align='left'>")
								if sidecontentArr(c,2) = "" or isnull(sidecontentArr(c,2)) then
									response.write "<img src='" & HTMLEncode(sidecontentArr(c,1)) & "' border='0' alt='" & sidecontentArr(c,3) & "' />"
								else
									Response.Write	"<a href='" & HTMLEncode(sidecontentArr(c,2)) & "'><img src='" & HTMLEncode(sidecontentArr(c,1)) & "' border='0' alt='" & sidecontentArr(c,3) & "' /></a><br />"
								end if
				Response.write("</td></tr></table>")
			elseif sidecontentArr(c,0) = 1 then				'this section is for a text area (w/header)
				Response.write(_
				"<table width='140' cellpadding='0' cellspacing='0' border='0' summary='Table for a text area (w/header)'>" &_
					"<tr>" &_
						"<td valign='top' rowspan='2' width='5'><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='5' height='5' alt='' /></td>" &_
						"<td valign='top' class='btsbRed'>" &_
							sidecontentArr(c,1) & "<br />" &_
							"<img width='135' height='6' src='" & GetImagePath("/global/lines/gydot135x6.gif") & "' border='0' alt='...............' /><br /></td></tr>" &_
					"<tr><td valign='top' class='SBText'>" &_
							 sidecontentArr(c,2) & "<br/></td></tr>" &_
				"</table>")
			elseif sidecontentArr(c,0) = 2 then				'this section is for text next to an arrow
				response.write(_
					"<table border='0' width='140' cellpadding='0' cellspacing='0' summary='Table for text next to an arrow'>" &_
						"<tr>" &_
							"<td width='15' align='center' valign='top'><a href='"& HTMLEncode(sidecontentArr(c,2)) &"'><img src='" & GetImagePath("/global/arrows/red_arrow.gif") & "' alt='&gt;' border='0' /></a></td>" &_
							"<td width='125' valign='top'><a href='"& HTMLEncode(sidecontentArr(c,2)) &"' class='SidebarLink' title='" & sidecontentArr(c,1) & "'>" & sidecontentArr(c,1) & "</a></td>" &_
						"</tr>" &_
					"</table>" )
			elseif sidecontentArr(c,0) = 3 then				'this section is for an image with text below it
				response.write(_
					"<table border='0' width='140' cellpadding='0' cellspacing='0' summary='Table for an image with text below it'>" &_
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
			"<table width='140' border='0' cellpadding='0' cellspacing='0' valign='top' Summary='Table for Custom Page Link'>"  &_
				"<tr>"  &_
					"<td colspan='3'>" &_
						"<img src='" & GetImagePath(glLabelPath & "CustomPage.gif") & "'  alt='Custom Page' />" &_
					"</td>"  &_
				"</tr>" )

		Response.Write(_
				"<tr>"  &_
					"<td width='5' valign='middle' align='left'><img width='5' height='1' src='" & GetImagePath("/global/pixels/none.gif") & "' alt='' /></td>"  &_
					"<td width='10' align='left' valign='top' class='MenuLink'>&#183;</td>"  &_
					"<td width='125' valign='top'>")
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
		dim bs, textcolor
		textcolor = "#000000"

		If glMacWarehouseUser = True Then
			Response.write (_
					"<table width='140' border='0' cellpadding='0' cellspacing='0' valign='top' summary='resource sidebar'>"  &_
						"<tr>"  &_
							"<td colspan=3>" &_
								"<img src='" & GetImagePath(glLabelPath & "MyMacWarehouse.gif") & "'  alt='My MacWarehouse'>" &_
							"</td>"  &_
						"</tr>" )

					Call DrawRowDivider()

					'Created a new sub for the menu items. Paul Haan 9/3/2004
					Call DrawMenuItem(glMyPurchasesURL, "My Purchases")
					Call DrawMenuItem(glOrderStatusURL, "Order Status")
					Call DrawMenuItem(glShoppingCartURL, "View Cart")
					Call DrawMenuItem("/shop/eaccount/eaccount.asp", "Account Details")

		Else
			Response.write (_
					"<table width='140' border='0' cellpadding='0' cellspacing='0' valign='top' summary='resource sidebar'>"  &_
						"<tr>"  &_
							"<td colspan=3>" &_
								"<img src='" & GetImagePath(glLabelPath & "label_resources.gif") & "'  alt='Resources'>" &_
							"</td>"  &_
						"</tr>" )

						'Created a new sub for the menu items. Paul Haan 9/3/2004
						Call DrawMenuItem(glMyPurchasesURL, "My Purchases")
						Call DrawMenuItem(glOrderStatusURL, "Order Status")
						Call DrawMenuItem(glMyOrganizationURL, "My Company")

						Call DrawMenuItem("/shop/eaccount/eaccount.asp", "My Account")
						Call DrawMenuItem("/shop/eaccount/accountteam.asp", "Account Team")
						If glCDWCanadaUser = False Then
							Call DrawMenuItem("/webcontent/inside/service/corporate_acct.asp", "New Accounts")
						End If
						Call DrawMenuItem(glRebateDefaultURL, "Rebates")
						Call DrawMenuItem("/r.asp?n=24012", "Special Events")

						If glCDWCanadaUser = False Then
							Call DrawMenuItem("/r.asp?n=28717", "CDW Outlet")
						End If

						Call DrawMenuItem("/support/default.asp", "Technical Support")

						If glCDWCanadaUser = False Then
							Call DrawMenuItem("/shop/tools/buyersedge/default.asp", "eNewsletters")
						End If

						Dim ResearchCenterLink, LinkText
						If glCDWCanadaUser Then
							ResearchCenterLink = "/webcontent/resource_ctr/resource_center.asp"
							LinkText			= "Research Centre"
						Else
							ResearchCenterLink = "/r.asp?n=35919"
							LinkText			= "Research Center"
						End If

						Call DrawMenuItem(ResearchCenterLink, LinkText)
		End If

		Response.write(_
			"</table>"  &_
			"<img src='" & GetImagePath("/global/pixels/none.gif") & "' height='16' width='1' alt='' /><br />" &_
			"<!-- End Resource Sidebar Code -->" )
end sub
'********************************* Write the footer *********************************
sub WriteFooter()
	Dim FilePath

	If glCDWCanadaUser Then
		FilePath	= "http://" & glMainSiteName & "/webcontent/insideCA/"
	Else
		FilePath	= "http://" & glMainSiteName & "/webcontent/inside/"
	End If

	if glFlagShowMainHeader then
		Response.Write  "<!-- Start Footer Code -->" 
		Response.write("<br /><br />")
		Response.write("<table border='0' cellspacing='0' cellpadding='0' summary='Table for footer' style='padding-bottom:5px;'>")
		Response.write("<tr>")
		if glEnableLeftSpacing = False then
			If glMacWarehouseUser  Then
				Response.write(_
				"<td rowspan='2'>" &_
					"<img src='" & GetImagePath("/global/pixels/none.gif") & "' width='12' height='1' alt='' />" &_
				"</td>" )
			Else
				Response.write("<td width='7' rowspan='2'><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='7' height='1' alt='' /></td>")
			End If
		end if

			Response.write(_
				"<td>" & _
					FooterFile & _
				"</td>" & vbCrLf &_
			"</tr>" & _
			"<tr>")

		Response.write("<td class='bts'>")

		If glTrueClosedLoop <> true then
			If glCDWCanadaUser Then
				Response.write("Copyright " & Year(Date) & " CDW Canada, Inc.<br />")
			Else
				Response.write("Copyright " & Year(Date) & " CDW Corporation<br />")
			End If

			'Write out the regular footer links
				Response.write(_
				"<a href='" & FilePath & "useragreement.asp' title='Terms and Conditions of Use'>Terms and Conditions of Use</A>&nbsp;" &_
					"|&nbsp;" &_
				"<a href='" & FilePath & "salesagreement.asp' title='Terms and Conditions of Sale'>Terms and Conditions of Sale</A>&nbsp;" &_
					"|&nbsp;" &_
					"<a href='" & FilePath & "privacy.asp' title='Privacy Policy'>Privacy Policy</A><br /> ")
			If glCDWCanadaUser Then
				Response.write("<span class='sstGrey'>All prices shown in Canadian dollars unless otherwise indicated.</span>")
			End If
		end if
	end if
		Response.write "</td></tr></table>"  
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
		Response.Write(_
			"<!-- Start Errors -->"  &_
			"<table width='100%' bgcolor='#EFEFEF' cellpadding='9' cellspacing=0 bordercolor='#ff00ff' border='0' summary='outside error message table'>"  &_
				"<tr>"  &_
					"<td width='100%'>"  & _
						"<table width='100%' cellpadding='0' cellspacing='0' border='0' summary='Main error message table'>"  &_
							"<tr>"  &_
								"<td rowspan='2'>" &_
									"<img src='" & GetImagePath("/global/critical.gif") & "' border='0' alt='Critical Error' /><br />" &_
								"</td>"  &_
								"<td rowspan='2'>" &_
									"<img src='" & GetImagePath("/global/pixels/none.gif") & "' width='10' height='1' border='0' /><br />" &_
								"</td>"  &_
								"<td width='100%' valign='top' class='bodyHeadLarge'>")

										If(IsArray(MainErrors)) Then
											Response.Write(MainErrors(RunTimeErrors,0))
										End If

		Response.Write(_
								"</td>"  &_
							"</tr>"  &_
						"</table>"  )

		Response.Write(_
						"<table width='100%' cellpadding='0' cellspacing='0' border='0' summary='Main error message table # 2'>"  &_
							"<tr>"  &_
								"<td class='bts'>" & _
									"<br /><img src='" & GetImagePath("img.cdw.com/global/pixels/none.gif") & "' width='42' height='1' border='0' />" )

									If(IsNumeric(RunTimeErrors)) Then
										If(IsArray(MainErrors)) Then
											Response.Write(MainErrors(RunTimeErrors,1))
										End If
									End If

		Response.Write(_
								"</td>"  &_
							"</tr>"  &_
							"<tr>"  &_
								"<td>" &_
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
								"<td class='bts'>" &_
										"Please correct the following:<br />" &_
								"</td>"  &_
							"</tr>"  &_
							"<tr>"  &_
								"<td>" &_
									"<img src='" & GetImagePath("/global/pixels/none.gif") & "' width='1' height='18' border='0' /><br />" &_	
								"</td>"  &_
							"</tr>"  &_
							"<tr>"  &_
								"<td width='100%' class='bts'>" &_
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

	dim dataArray(8,2)

		dataArray(0,0) = "MyCompany"
		dataArray(0,1) = ""
		dataArray(0,2) = False
		dataArray(1,0) = "Account Linking"
		dataArray(1,1) = "http://" & request.ServerVariables("SERVER_NAME") & "/shop/tools/accountlinking/default.aspx"
		dataArray(1,2) = False
		dataArray(2,0) = "Accounts Payable"
		dataArray(2,1) = "http://" & request.ServerVariables("SERVER_NAME") & "/shop/tools/purchasehistory/orderDownloadForm.asp"
		dataArray(2,2) = False
		dataArray(3,0) = "Asset Management"
		dataArray(3,1) = "/shop/assets/default.asp"
		dataArray(3,2) = False
		dataArray(4,0) = "Software License Tracker"
		dataArray(4,1) = "/shop/software/default.aspx"
		dataArray(4,2) = False
		dataArray(5,0) = "Employee Purchases"
		dataArray(5,1) = "/shop/EPP/setup/default.aspx"
		dataArray(5,2) = False
		dataArray(6,0) = "Quotes"
		dataArray(6,1) = "/shop/quotes/default.aspx"
		dataArray(6,2) = False
		dataArray(7,0) = "Purchase Authorization"
		dataArray(7,1) = "/shop/pas/default.asp"
		dataArray(7,2) = False
		dataArray(8,0) = "Online Chat"
		dataArray(8,1) = "/shop/eaccount/ChatMain.aspx?Pp=1"
		dataArray(8,2) = False

	sbArray = dataArray
End Function

' Write the array for the MyAccount Links
function Get_MyAccountLinks(sbArray)
	dim dataArray(4,2)

		dataArray(0,0) = "MyAccount"
		dataArray(0,1) = ""
		dataArray(0,2) = False
		dataArray(1,0) = "Change Password"
		dataArray(1,1) = "/shop/eaccount/setuserpassword.asp?target=/shop/eaccount/eaccount.asp"
		dataArray(1,2) = False
		dataArray(2,0) = "Secret Question"
		dataArray(2,1) = "/shop/eaccount/setuserhints.asp?target=/shop/eaccount/eaccount.asp"
		dataArray(2,2) = False
		dataArray(3,0) = "My Solutions"
		dataArray(3,1) = "/shop/tools/customcatalog/individual/default.asp"
		dataArray(3,2) = False
		dataArray(4,0) = "My Subscriptions"
		dataArray(4,1) = "/shop/eaccount/usercontact.asp?target=/shop/eaccount/eaccount.asp"
		dataArray(4,2) = False

	sbArray = dataArray
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
	dataArray(1,1) = "/support/default.asp"
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
						"<img src='" & GetImagePath("/global/parts/tabs/blue_left.gif") & "' alt=' '>" & _
					"</td>" &_
					"<td bgcolor='#cccccc' height='22' valign='middle' width='132' background='" & GetImagePath("/global/parts/tabs/blue_mid.gif") & "'>" & _
						"<img src='" & GetImagePath("/macWarehouse/hubs/labels/" & fileName ) & "' alt='" & altTag & "'>" & _
					"</td>" &_
					"<td align='right' valign='top' height='22' width='7'>" & _
						"<img src='" & GetImagePath("/global/parts/tabs/blue_right.gif") & "' alt=''>" & _
					"</td>" &_
				"</tr>" & VbCrLf &_
			"</table>" & VbCrLf)

		ReturnValue = (ReturnValue & _
			"<table width='146' height='" & (height - 22) & "' border=0' cellspacing='0' cellpadding='0' bordercolor='#CE0000'>" &_
			"<tr height='" & (height - 28) & "'>" & _
				"<td width='1' bgcolor='#999999'>" & _
					"<img src='" & GetImagePath("/global/pixels/none.gif") & "' width='1' height='1' alt=''>" & _
				"</td>" & _
				"<td width='6'>" & _
					"<img src='" & GetImagePath("/global/pixels/none.gif") & "' width='6' height='1' alt=''>" & _
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
				"<td width='1' bgcolor='#999999'>" & _
					"<img src='" & GetImagePath("/global/pixels/none.gif") & "' width='1' height='1' alt='' />" & _
				"</td>" & _
			"</tr>" & VbCrLf & _
			"<tr height='6'>" & _
				"<td colspan='2' height='6' width='7' valign='bottom'>" & _
					"<img src='" & GetImagePath("/global/parts/corners/btm_left.gif") & "' height='6' width='6'  alt='bottom border'>" & _
					"<img src='" & GetImagePath("/global/pixels/999999.gif") & "' height='1' width='1' alt='bottom border'>" & _
				"</td>" & _
				"<td height='6' valign='bottom'><img src='" & GetImagePath("/global/pixels/999999.gif") & "' width='100%' height='1' alt='bottom border'></td>" & _
				"<td colspan='2' height='6' width='7' valign='bottom'>" & _
					"<img src='" & GetImagePath("/global/pixels/999999.gif") & "' height='1' width='1' alt='bottom border'>" & _
					"<img src='" & GetImagePath("/global/parts/corners/btm_right.gif") & "' height='6' width='6' alt='bottom border'>" & _
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
	Dim URLPath, ImageSrc, ReturnValue
	If lCase(Request.ServerVariables("HTTPS")) = "on" Then
		URLPath = "https://webobjects.cdw.com/staticimg"
	Else
		URLPath = "http://webobjects.cdw.com/staticimg"
	End If
		ImageSrc	= URLPath & "/thumbs/" & HTMLEncode(left(EDC,1)) & "/" & HTMLEncode(mid(EDC,2,1)) & "/" & HTMLEncode(trim(mid(EDC,3))) & ".gif"

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
				"<a href='/r.asp?n=" & RedirectNumber & "' class='NGbody'>")

				If IsBold = True Then ReturnValue = ReturnValue & ("<span style='font-weight: bold;'>") End If
					ReturnValue = ReturnValue & (text)
				If IsBold = True Then ReturnValue = ReturnValue & ("</span>") End If

	ReturnValue = ReturnValue & (_
				"</a>" & _
			"</td>" & _
		"</tr>" & VbCrLf)

	DrawPageLink = ReturnValue
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

Sub DrawMenuItem(link, name)
	Response.write(_
		"<tr>"  &_
			"<td width='5' valign='middle' align='left'><img width='5' height='1' src='" & GetImagePath("/global/pixels/none.gif") & "' alt='' /></td>"  &_
			"<td width='10' align='left' valign='top' class='MenuLink'>&#183;</td>"  &_
			"<td width='125' valign='top'>")
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