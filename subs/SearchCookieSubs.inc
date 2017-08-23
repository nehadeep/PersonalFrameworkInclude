<%
'****************************************************************************************************************************************************
' SearchCookieSubs.inc

' This file contains utility funciton related to maintaining a list of search cookies.
'****************************************************************************************************************************************************
const SearchCookies = "CDWSearch"
const maxSearchCookies = 5
const CookiePersistenceInDays = 30
dim ArrMasterSearchCookie
'***************************************************************************************************************************
' Called when saving a search into a cookie

' ID is a subcookie of the CDWSearch cookie that keeps track of what search to overwrite if the user is interested in
' saving more than the maximum number of searches. We are using a FIFO cookie approach.
'***************************************************************************************************************************
function SaveSearchInCookie(keyword)
	dim ID, Cookie, count, i, alreadyPresent
	dim dataarray(6) 'MaxSearchCookies + 1

	'count = Request.Cookies(SearchCookies).Count
	'if count >= maxSearchCookies then count = maxSearchCookies - 1
	alreadyPresent = false

	count = maxSearchCookies - 1

	' get all the cookies
	For i = count to 1 step - 1
		DataArray(i) = Request.Cookies(SearchCookies)( CStr(i) )

		if keyword = DataArray(i) then alreadyPresent = true
	next

	if alreadyPresent = false then
		count = maxSearchCookies - 1
		'cycle through the cookies from the last one and push them down
		For i = count to 1 step - 1
			' push everything up 1 (4 becomes 5, 3 becomes 4, etc) 1 should be clear
			'Response.Cookies(SearchCookies)( CStr(i+1) ) = DataArray( i )
			'Response.Cookies(SearchCookies).Path = "/"
			'Response.Cookies(SearchCookies).Expires = DateAdd("d",now(),CookiePersistenceInDays)
            Call SetMasterCookie(SearchCookies, ArrMasterSearchCookie, CStr(i+1), DataArray(i), DateAdd("d",now(),CookiePersistenceInDays))
		next 
		'Response.Cookies(SearchCookies)("1") = keyword
        Call SetMasterCookie(SearchCookies, ArrMasterSearchCookie, "1", keyword, DateAdd("d",now(),CookiePersistenceInDays))
	end if
end function

'***************************************************************************************************************************
' Reutnrs an array of stuff used by the left column display functions
function getSavedSearches
	dim SavedSearches, i, Key, temp
	dim dataarray(5,2) 'MaxSearchCookies + 1

	SavedSearches = true

	dataArray(0,0) = "PastSearches"
	dataArray(0,1) = ""
	dataArray(0,2) = False

	if Request.Cookies(SearchCookies).Count = 0 then SavedSearches = false

	For i = 1 to Request.Cookies(SearchCookies).Count
		key = Request.Cookies(SearchCookies)(i)

		temp = Trim( Request.Cookies(SearchCookies)( CSTR(i) ) )

		'only put stuff in if the cookie is not blank
		if len(temp) > 0 then
			if len(temp) > 15 then
				dataArray(i,0) = Mid( temp, 1,15 ) & "..."
			else
				dataArray(i,0) = temp
			end if

			dataArray(i,1) = CStr("/shop/search/" & GetSearchPage() & "?Key=" & Server.URLEncode(temp) )
			dataArray(i,2) = false
		end if
	next 

	if SavedSearches = true then
		If glClosedLoop Then
			getSavedSearches = null
		Else
			getSavedSearches = dataArray
		End If
	else
		getSavedSearches = null
	end if
end function
%>