<%
	'DESCRIPTION:
	'	Contains functions related to parsing out information from the url+querystring
	'		or putting information onto a url
	'	And functions for getting url's for other pages.
	'ASSUMPTIONS:
	'	"/global/subs/global.inc"	

	Function GetProductURL(EDC)
		dim CurrentSite
		GetProductURL = null
		If Not IsNull(EDC) Then
			GetProductURL = glProductURL & "?EDC=" & EDC
		end if
	End Function
%>