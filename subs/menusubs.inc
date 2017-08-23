<!--#INCLUDE VIRTUAL="/global/subs/WebSiteSubs.inc"-->
<%
'CHANGE HISTORY:
'	Paul Haan (01/10/2006)
'	Changed the CrossBrowserMenus.js file to be called from Code instead of Content.
'	That file is definitely code...

'Descrption
'	Contains routines to render cross browser drop down menus based on database information.

Const MenuSubs_CdwMenu			= 1000
Const MenuSubs_CdwMacMenu		= 1001
Const MenuSubs_CdwGMenu			= 1002
Const MenuSubs_CdwCanadaMenu	= 1003

	if glShowHeaderOnly Then
		targettop = "target='_top'"
	else
		targettop = ""
	end if

Dim MenuSubs_HasJavascriptBeenInserted
MenuSubs_HasJavascriptBeenInserted = false

dim MenuSubs_ParentMenuItems
MenuSubs_ParentMenuItems = null

dim MenuSubs_MenuSettings
MenuSubs_MenuSettings = null

Sub MenuSubs_InsertJavascript(WebSite_MenuID, UsePaddingRow)
	If MenuSubs_HasJavascriptBeenInserted <> True Then
		If UsePaddingRow = True Then
			Response.Write("<script src='/SubMenuGenerator.aspx?UsePaddingRow=1&WebSite_MenuID=" & WebSite_MenuID & "' language='javascript'></script>")
		Else
			Response.Write("<script src='/SubMenuGenerator.aspx?WebSite_MenuID=" & WebSite_MenuID & "' language='javascript'></script>")
		End If

		Response.Write("<script src='/global/gui/javascript/CrossBrowserMenus.js' language='javascript'></script>")

		MenuSubs_HasJavascriptBeenInserted = true
	End If
End Sub

Sub MenuSubs_GenerateTabs(WebSite_MenuID)
		dim i, j
		dim MenuItemName, MenuItemUrl
		dim Menu, MenuBackgroundColor, MenuDividerColor, SubMenuBorderColor, MenuHeight, MenuBackgroundGraphic, SubMenuBackgroundColor, MenuBottomBorderColor
		dim MenuTabCssClass, MenuTabLinkCssClass, MenuTabHeight
		dim MenuItemBGImage, MenuItemTextOffset, MenuItemFontStyle, MenuItemBackgroundImageString

		If MenuSubs_HasJavascriptBeenInserted = False Then
			Err.Raise 101, "menusubs.inc", "Make sure you call MenuSubs_InsertJavascript() before calling MenuSubs_GenerateTabs()" 
		End If

		MenuSubs_MenuSettings = WebSite_GetMenu(WebSite_MenuID)
		If IsNull(MenuSubs_MenuSettings) = True Then
			Err.Raise 101, "menusubs.inc", "Menu couldn't be loaded, make sure you specified a proper menu id from WebDB.dbo.WebSite_Menu" 
		End If

		MenuBackgroundColor		= MenuSubs_MenuSettings(wsgmMenuBackgroundColor)
		MenuDividerColor		= MenuSubs_MenuSettings(wsgmMenuDividerColor)
		SubMenuBorderColor		= MenuSubs_MenuSettings(wsgmSubMenuBorderColor)
		MenuHeight				= MenuSubs_MenuSettings(wsgmMenuHeight)
		MenuBackgroundGraphic	= MenuSubs_MenuSettings(wsgmMenuBackgroundGraphic)
		SubMenuBackgroundColor	= MenuSubs_MenuSettings(wsgmSubMenuBackgroundColor)
		MenuBottomBorderColor	= MenuSubs_MenuSettings(wsgmMenuBottomBorderColor)
		MenuTabCssClass			= MenuSubs_MenuSettings(wsgmMenuTabCssClass)
		MenuTabLinkCssClass		= MenuSubs_MenuSettings(wsgmMenuTabLinkCssClass)
		MenuTabHeight			= MenuSubs_MenuSettings(wsgmMenuTabHeight)

		MenuSubs_ParentMenuItems = WebSite_GetMenuItems(WebSite_MenuID, null)

		If IsNull(MenuSubs_ParentMenuItems) = False Then
			Response.Write("<div onMouseOver=""if(DDM_IsLoaded == false) return false; menuOver(); event.cancelBubble = true;"">")
			Response.Write("<table cellpadding='0' cellspacing='0' border='0' height='" & MenuTabHeight & "' bgcolor='#aaaaaa'>")
				Response.Write("<tr>")

			' Write out the menu tabs.
			For i = Lbound(MenuSubs_ParentMenuItems, 2) To Ubound(MenuSubs_ParentMenuItems, 2)
				MenuItemName = MenuSubs_ParentMenuItems(wsgmiName, i)
				MenuItemUrl = MenuSubs_ParentMenuItems(wsgmiUrl, i)
				MenuItemBGImage = MenuSubs_ParentMenuItems(wsgmiBackgroundGraphic, i)
				MenuItemTextOffset = MenuSubs_ParentMenuItems(wsgmiTextOffset, i)
				MenuItemFontStyle = MenuSubs_ParentMenuItems(wsgmiFontStyle, i)
				
'Response.write("MenuItemBGImage:  " & MenuItemBGImage & "<hr />")

					Response.Write("<td nowrap id='DDMAnchor_" & i + 1 & "' bgcolor='" & MenuDividerColor & "' width='1' height='" & MenuTabHeight & "'>")
						Response.Write("<img src='" & GetImagePath("/global/pixels/none.gif") & "' height='" & MenuTabHeight & "' width='1' alt=''>")
					Response.Write("</td>")

				If Not IsNull(MenuBackgroundGraphic) And Len(MenuBackgroundGraphic) > 0 Then
					MenuItemBackgroundImageString = GetImagePath(MenuBackgroundGraphic)

					If Not IsNull(MenuItemBGImage) And Len(MenuItemBGImage) > 0 Then
						MenuItemBackgroundImageString = GetImagePath(MenuItemBGImage)
'						MenuItemBackgroundImageString = "http://img.beta.cdw.com/global/header/ProductFinders_Nav.gif" 
					End If

					Response.write("<td nowrap background='" & MenuItemBackgroundImageString & "' style='padding-right: 4px; padding-left: ")

					If Not IsNull(MenuItemTextOffset) And Len(MenuItemTextOffset) > 0  Then
						Response.write( MenuItemTextOffset & "px; ")
					Else
						Response.write(" 4px;")
					End If
					If Not IsNull(MenuItemFontStyle) And Len(MenuItemFontStyle) > 0  Then
						Response.write(MenuItemFontStyle)
					End If
					Response.write("' class='menuRegular' height='" & MenuTabHeight & "' onMouseover=""if(DDM_IsLoaded == false) return false; return !showMenu('" & i + 1 & "', event, 'DDMLabel_" & i + 1 & "'); menuOver();"">")
				Else
					Response.Write("<td nowrap class='" & MenuTabCssClass & "' height='" & MenuTabHeight & "' onMouseover=""if(DDM_IsLoaded == false) return false; return !showMenu('" & i + 1 & "', event, 'DDMLabel_" & i + 1 & "'); if(DDM_IsLoaded == false) return false; menuOver();"">")
				End If

				If IsNull(MenuItemUrl) = false Then
						Response.Write("<a href='" & MenuItemUrl & "' ")
					If Not IsNull(MenuItemFontStyle) And Len(MenuItemFontStyle) > 0  Then
						Response.write(" style='" & MenuItemFontStyle & "'")
					Else
						Response.write("class='" & MenuTabLinkCssClass & "' ")
					End If
						Response.write( targettop & ">")
				Else
						Response.Write("<a href='#' class='ProductMenu'>")
				End If

						Response.Write(MenuItemName & "</a>")

					Response.Write("</td>")
			Next

					Response.Write("<td nowrap bgcolor='" & MenuDividerColor & "' width='1' height='" & MenuTabHeight & "'>")
						Response.Write("<img src='" & GetImagePath("/global/pixels/none.gif") & "' height='1' width='1' alt=''>")
					Response.Write("</td>")

				Response.Write("</tr>")
			Response.Write("</table>")
			Response.Write("</div>")

'				Response.Write("<tr>")

'			' Write out the sub menu containers. These containers will be filled by the 
'			' the sub menu dynamic javascript. 
'			For i = Lbound(MenuItems, 2) To Ubound(MenuItems, 2)
'					Response.Write("<td nowrap bgcolor='" & MenuDividerColor & "' width='1px' height='1px'>")
'						'Response.Write("<img src='" & GetImagePath("/global/pixels/none.gif") & "' height='1' width='1' alt=''>")
'						Response.Write("<div id='DDMMenu_" & i + 1 & "' style='position:absolute;z-index:100;visibility:hidden' onmouseover='event.cancelBubble = true;' onmouseout='menuOut(""rollimg2"");'>")

''						Response.Write("<script language='javascript'>" & vbcrlf)
'							Response.Write("<!--" & vbcrlf)
'							Response.Write("if(document.getElementById)" & vbcrlf)
'							Response.Write("RenderSubMenu(" & i + 1 & ");" & vbcrlf)
'							Response.Write("//-->" & vbcrlf)
'						Response.Write("</script>" & vbcrlf)

'						Response.Write("</div>")

'					Response.Write("</td>")

'					Response.Write("<td valign='top' align='left' nowrap bgcolor='" & MenuBottomBorderColor & "'>")
'						Response.Write("<img src='" & GetImagePath("/global/pixels/none.gif") & "' height='1' width='1' alt=''>")
'					Response.Write("</td>")
'			Next

'					Response.Write("<td nowrap bgcolor='#444444' width='1px' height='1px'>")
'						Response.Write("<img src='" & GetImagePath("/global/pixels/none.gif") & "' height='1' width='1' alt=''>")
'					Response.Write("</td>")
'				Response.Write("</tr>")
'			Response.Write("</table>")
		End If
End Sub

Sub MenuSubs_GenerateSubMenus(WebSite_MenuID)
	Response.Write("<!--SubMenus Begin-->" & vbcrlf)

	dim i
	dim MenuItemName, MenuItemUrl
	dim MenuBackgroundColor, MenuDividerColor, SubMenuBorderColor, MenuHeight, MenuBackgroundGraphic, SubMenuBackgroundColor, MenuBottomBorderColor

	If IsNull(MenuSubs_MenuSettings) = True Then
		Exit Sub
		'Err.Raise 101, "menusubs.inc", "Menu settings were not loaded, make sure you called MenuSubs_GenerateTabs() first." 
	End If

	MenuBackgroundColor		= MenuSubs_MenuSettings(wsgmMenuBackgroundColor)
	MenuDividerColor		= MenuSubs_MenuSettings(wsgmMenuDividerColor)
	SubMenuBorderColor		= MenuSubs_MenuSettings(wsgmSubMenuBorderColor)
	MenuHeight				= MenuSubs_MenuSettings(wsgmMenuHeight)
	MenuBackgroundGraphic	= MenuSubs_MenuSettings(wsgmMenuBackgroundGraphic)
	SubMenuBackgroundColor	= MenuSubs_MenuSettings(wsgmSubMenuBackgroundColor)
	MenuBottomBorderColor	= MenuSubs_MenuSettings(wsgmMenuBottomBorderColor)

	If IsNull(MenuSubs_ParentMenuItems) = False Then
		' Write out the menu tabs.
		For i = Lbound(MenuSubs_ParentMenuItems, 2) To Ubound(MenuSubs_ParentMenuItems, 2)
			Response.Write("<div id='DDMMenu_" & i + 1 & "' style='position:absolute;width:1px;height:1px;overflow:hidden;z-index:100;visibility:hidden' onmouseover=""if(DDM_IsLoaded == false) return false; menuOver(); event.cancelBubble = true;"">")	
			Response.Write("<script language='javascript'>" & vbcrlf)
				Response.Write("<!--" & vbcrlf)
				Response.Write("if(document.getElementById)" & vbcrlf)
				Response.Write("RenderSubMenu(" & i + 1 & ");" & vbcrlf)
				Response.Write("//-->" & vbcrlf)
			Response.Write("</script>" & vbcrlf)
			Response.Write("</div>")
		Next
	End If

	Response.Write("<!--SubMenus End-->" & vbcrlf)
End Sub
%>