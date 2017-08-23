<%
	'DESCRIPTION :
	'	Contains URL's for webservice addresses for the as400 hosted web services
	'
	'ASSUMPTIONS:
	' global/subs/global.inc

	Function GetWebServiceURL()	
		'determine which web service address to return based on development mode
		Dim returnValue, DevelopmentMode
	
		returnValue = Null
		DevelopmentMode = GetSiteDevelopmentMode()
		
		Select Case DevelopmentMode
			Case glLocalMode
				returnValue = "http://Idev01.corp.cdw.com:8450/glue"
			Case glBetaMode
				returnValue = "http://Idev01.corp.cdw.com:8450/glue"
			Case glQAMode
				returnValue = "http://Idev01.corp.cdw.com:8500/glue"
			Case glStagingMode
				returnValue = "http://Idev01.corp.cdw.com:8585/glue"
			Case glProductionMode
				returnValue = "http://webservice-publisher.wip.web.cdw.com.:8484/glue"
		End Select
		
'		If DevelopmentMode <> glProductionMode And (Request.QueryString("ForceProductionWS") = "1"  Or Request.Form("ForceProductionWS") = "1")Then
'			returnValue = "http://zorro:8484/glue"
'		End If
		
		GetWebServiceURL = returnValue
	End Function						
	


	'DESCRIPTION :
	'	Contains URL's for webservice addresses for the .Net hosted web services
	'	load balanced : http://webservice.wip.corp.cdw.com
	'
	'ASSUMPTIONS:
	' global/subs/global.inc	
	
	Function GetWebServiceDotNetURL()	
		'determine which web service address to return based on development mode
		Dim returnValue, DevelopmentMode
	
		returnValue = Null
		DevelopmentMode = GetSiteDevelopmentMode()
		
		Select Case DevelopmentMode
		
			Case glBetaMode
				returnValue = "http://websvcbeta"
			Case glQAMode
				returnValue = "http://webserviceqa1"
			Case glStagingMode
				returnValue = "http://webservicestage"
			Case glProductionMode
				returnValue = "http://webservice"
		End Select
		
		GetWebServiceDotNetURL = returnValue
	End Function	
	
	
	
	
	
				
%>							