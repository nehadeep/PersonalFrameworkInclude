<%

Sub DrawInnerFrameSet(FramesetPageTitle, FrameSetBodyURL, NoFramesText, Height) 	
	Response.Write(_		
			"<iframe id='iframeFinder' name='iframeFinder' src='" & FrameSetBodyURL & "' frameborder='no' width='100%' height='" & Height & "px'></iframe>" & _
			"<noframes>" & NoFramesText & "</noframes>")
End Sub

'***********************************************************************************************************************
' The following sub procedure creates a simple 3 section frameset (Header, Body, Footer)
'	It will be used on a majority of the Configurators that we have framed on our site.
'	Pass in the Title, URL and Text message for users that don't have frames
Sub DrawThreeSectionFrameSet(FramesetPageTitle, FrameSetBodyURL, NoFramesText)
	Dim HeaderHeight, FooterHeight

		HeaderHeight	= 113
		FooterHeight	= 50

	If glWebSiteMode <> glProductionMode Then
		HeaderHeight = (HeaderHeight + 25)
		FooterHeight = (FooterHeight + 25)
	End If

	Response.write(_
		"<!DOCTYPE HTML PUBLIC ""-//W3C//DTD HTML 3.2 Final//EN"">" & _
		"<HTML>" & _
			"<HEAD>" & _
				"<TITLE>" & FramesetPageTitle & "</TITLE>" & _
			"</HEAD>" & _
			"<frameset rows='" & HeaderHeight & ", *, " & FooterHeight & "' frameborder='0'>" & _
				"<frame name='header' src='/global/gui/header.asp' scrolling='no' frameborder='0' noresize>" & _
				"<frame name='body' src='" & FrameSetBodyURL & "' scrolling='Auto' frameborder='0' noresize>" & _
				"<frame name='footer' src='/global/gui/footer.asp' scrolling='no'  marginwidth=0 marginheight=0 " & _
					"frameborder='0' bordercolor='#CCCCCC' noresize>" & _
			"</frameset>" &_
			"<noframes>" & NoFramesText & "</noframes>" & _
			"<BODY></BODY>" & _
		"</HTML>")
End Sub

Sub DrawThreeSectionPetiteFrameSet(FramesetPageTitle, FrameSetBodyURL, NoFramesText)
	Dim HeaderHeight, FooterHeight

		HeaderHeight	= 62
		FooterHeight	= 50

	If glWebSiteMode <> glProductionMode Then
		HeaderHeight = (HeaderHeight + 25)
		FooterHeight = (FooterHeight + 25)
	End If

	Response.write(_
		"<!DOCTYPE HTML PUBLIC ""-//W3C//DTD HTML 3.2 Final//EN"">" & _
		"<HTML>" & _
			"<HEAD>" & _
				"<TITLE>" & FramesetPageTitle & "</TITLE>" & _
			"</HEAD>" & _
			"<frameset rows='" & HeaderHeight & ", *, " & FooterHeight & "' frameborder='0'>" & _
				"<frame name='header' src='/global/gui/petiteheader.asp' scrolling='no' frameborder='0' noresize>" & _
				"<frame name='body' src='" & FrameSetBodyURL & "' scrolling='Auto' frameborder='0' noresize>" & _
				"<frame name='footer' src='/global/gui/footer.asp' scrolling='no'  marginwidth=0 marginheight=0 " & _
					"frameborder='0' bordercolor='#CCCCCC' noresize>" & _
			"</frameset>" &_
			"<noframes>" & NoFramesText & "</noframes>" & _
			"<BODY></BODY>" & _
		"</HTML>")
End Sub

'This is for Popup pages that WILL NOT need the footer.
Sub DrawTwoSectionPetiteFrameSet(FramesetPageTitle, FrameSetBodyURL, NoFramesText)
	Dim HeaderHeight, FooterHeight
		HeaderHeight	= 62

		If glWebSiteMode <> glProductionMode Then
			HeaderHeight = (HeaderHeight + 25)
		End If

	Response.write(_
		"<!DOCTYPE HTML PUBLIC ""-//W3C//DTD HTML 3.2 Final//EN"">" & _
		"<HTML>" & _
			"<HEAD>" & _
				"<TITLE>" & FramesetPageTitle & "</TITLE>" & _
			"</HEAD>" & _
			"<frameset rows='" & HeaderHeight & ", *' frameborder='0'>" & _
				"<frame name='header' src='/global/gui/petiteheader.asp' scrolling='no' frameborder='0' noresize>" & _
				"<frame name='body' src='" & FrameSetBodyURL & "' scrolling='Auto' frameborder='0' noresize>" & _
			"</frameset>" &_
			"<noframes>" & NoFramesText & "</noframes>" & _
			"<BODY></BODY>" & _
		"</HTML>")
End Sub
'***********************************************************************************************************************
%>