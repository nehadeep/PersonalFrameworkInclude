<%
    'Redirect shipstat.asp to FAQ page
    'Implement a 301 Moved Permanently status
    
    Response.Status="301 Moved Permanently"
    Response.AddHeader "Location", "http://www.cdwsiteinfo.com/about-our-site/faqs/"
    
%>