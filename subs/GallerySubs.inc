<%
' This sub writes out the option tags for the search drop down menu
sub WriteOptionTag(mfgCode, SelectedClass)
	dim rsClass, cmdClass, temp

	Call OpenSQLConnection(ctReadOnly)

	set cmdClass = CreateSQLProcedure(ctReadOnly, "WebDB.dbo.Product_GetClassesForMfg")
	cmdClass.Parameters.Append cmdClass.CreateParameter("@MfgCode",adChar,adParamInput,3,mfgCode)
	set rsClass = cmdClass.Execute

	do while not rsClass.EOF
		temp = rsClass(0)

		if temp = SelectedClass then
			Response.Write("<OPTION VALUE='" & HTMLEncode(temp) & "' SELECTED>" & HTMLEncode(rsClass(1)) & "</OPTION>" & vbcrlf)
		else
			Response.Write("<OPTION VALUE='" & HTMLEncode(temp) & "'>" & HTMLEncode(rsClass(1)) & "</OPTION>" & vbcrlf)
		end if

		rsClass.MoveNext
	loop

	set rsClass = Nothing
	set cmdClass = Nothing

	Call CloseSQLConnection(ctReadOnly)
end sub

function WriteOutSearchSelect(mfgCode, selectedClass)
	Response.write(_
	"<table border=0 cellpadding=0 cellspacing=0>" & _
		"<tr>" &_
			"<td><FORM ACTION='/shop/tools/sbb/vendor.asp' METHOD=get></td>" & _
			"<td valign='middle'><INPUT TYPE='hidden' Name='MFG' VALUE='" & mfgCode & "'>" &_
				"<SELECT NAME='SearchClass' onChange='SearchClass.form.submit();' >" &_
					"<OPTGROUP label='Product Groups'>")

					'If user has selected something then there is no reason to display this prompt as an option
						If len(selectedClass) = 0 then
							Response.write(	"<OPTION VALUE=''>Select product group </OPTION>" & vbcrlf )
						end if

					Call WriteOptionTag(mfgCode, SelectedClass)

		Response.write(_
					"</OPTGROUP>" & _
				"</SELECT>&nbsp;" & vbcrlf &_
			"</td>" & _
			"<td valign='middle'><INPUT TYPE='image' src='" & GetImagePath("/global/buttons/gy_go.gif") & "' BORDER='0' ALT='go'></td>" &_
			"<td></form></td>" & vbcrlf & _
		"</tr>" & _
	"</table>")
			
end function
%>