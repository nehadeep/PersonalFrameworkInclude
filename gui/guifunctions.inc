<%
'************************************************************************
'	DESCRIPTION:
'		Draw the header for the page with Grey text and the Company logo
'		Pass in a text string

	function drawTextHeadandLogo(OrganizationCode, OrganizationName, text)
		Dim OrgLogo
		if Not(isnull(OrganizationCode)) and Not(isnull(OrganizationName)) then
			OrgLogo = GetOrganizationImageTag(OrganizationCode, OrganizationName)
			Response.write(_
				"<table border=0 width='100%'>" &_
					"<TR>" &_
						"<TD valign='middle' class='bodyHeadLargeGrey'>" & HTMLEncode(text) & "</td>" &_
						"<TD align='right'>" &_
							 OrgLogo & "<BR>" &_
							"<span class='btsGrey'>Customized Extranet<BR></span>" &_
						"</td>" &_
					"</tr>" &_
				"</table>")
		end if
	end function
'************************************************************************
%>