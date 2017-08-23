<%
'****************************************************************************************************************************************************
' ProductSearchSubs.inc

' This file contains the search functions that are not specific to any one type of searchs.
'****************************************************************************************************************************************************
' Sometimes a word is bad. It can either have a replacement, or an alias to be used in addition to the word.

' strOutputword will contain the word to search on. If there is an alias word _in addition_ to the main word, then
' strAliasWord will contain it.

function AnalyzeWord( strInputWord, byRef strOutputWord, byRef strAliasWord )
	dim cmd, IgnoreWord, AliasWord, ReplaceWord, strTempOutputWord, strTempAliasWord, bManipulationPerformed
	dim strTempInputWord

	strTempOutputWord = null
	strTempAliasWord = null
	bManipulationPerformed = false
	strTempInputWord = UCASE(strInputWord)

	'lets get smart and try to give the users what they really want. Add additional global word rules here
  	'If Right(strTempInputWord,3) = "IES" then strTempInputWord = Left(strTempInputWord,Len(strTempInputWord) - 3) 'strip off trailing 'IES'
  	'If Right(strTempInputWord,2) = "ES" then strTempInputWord = Left(strTempInputWord,Len(strTempInputWord) - 2) 'strip off trailing 'ES'
	'There are legitimate queries that can end with S, like "WINDOWS" or "UPS"
	'If Right(strTempInputWord,1) = "S"   then strTempInputWord = Left(strTempInputWord,Len(strTempInputWord) - 1) 'strip off trailing 'S'

	If(Not(IsEmpty(strInputWord)) and Not(IsNull(strInputWord)) and Len(strInputWord) > 0) Then
		'Look up any replacements or alias for this word. Don't use the modified word, just in case
		'the modified version of the word would cause it not to have an alias. example 'MS' translates
		'to Micosoft, strip off the 'S' and you don't want to do a search on 'M'
		OpenROConnection

		Set cmd = CreateROStoredProcCommand("WebDB.dbo.Search_GetWordAlias")
			cmd.Parameters.Append cmd.CreateParameter("@Word",adVarChar,adParamInput,100)
			cmd.Parameters.Append cmd.CreateParameter("@IgnoreWord",adBoolean,adParamOutput)
			cmd.Parameters.Append cmd.CreateParameter("@AliasWord",adVarChar,adParamOutput,50)
			cmd.Parameters.Append cmd.CreateParameter("@ReplaceWord",adBoolean,adParamOutput)

			cmd.Parameters(1) = strInputWord
			cmd.Execute

		IgnoreWord	= cmd.Parameters(2)
		AliasWord	= cmd.Parameters(3)
		ReplaceWord	= cmd.Parameters(4)

		CloseROConnection

		if IgnoreWord = true then 
			strTempOutputWord = null
			strTempAliasWord = null
		else
			if ReplaceWord then
				strTempOutputWord = AliasWord
				strTempAliasWord = null
			else
				strTempOutputWord = strTempInputWord

				' check if we should use an alias word in addition to the input word
				if not ISNULL(AliasWord) then 
					strTempAliasWord = AliasWord
				else
					strTempAliasWord = null
				end if
			end if
		end if
	End If
	strOutputWord = strTempOutputWord
	strAliasWord = strTempAliasWord
end function
'*************************************************************************************************************************************************
' Given a querystring collection, this puts the collection into an array

function putCollectionIntoArray(CollectionName)
	dim arraysize, returnValue, i

	ReturnValue = null

	arraysize = 0
	arraysize = Request(CollectionName).count

	redim returnValue(arraysize-1)

	for i = 1 to Request(CollectionName).Count
		ReturnValue(i-1) = Request(CollectionName)(i)
	next

	putCollectionIntoArray = ReturnValue
end function
'****************************************************************************************************************************************************
function logSearch(key, qs, ProductsReturned, CategoriesReturned, ManufacturersReturned)
	Dim Cmd

	' just a simple database call
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Search_AddLogEntry")
	Cmd.Parameters.Append Cmd.CreateParameter("@Key", adChar, adParamInput, 32, key)			
	Cmd.Parameters.Append Cmd.CreateParameter("@QS", adVarChar, adParamInput, 500, qs)			
	Cmd.Parameters.Append Cmd.CreateParameter("@Products", adInteger, adParamInput,, ProductsReturned)			
	Cmd.Parameters.Append Cmd.CreateParameter("@Mfgs", adInteger, adParamInput,, categoriesReturned)			
	Cmd.Parameters.Append Cmd.CreateParameter("@Folders", adInteger, adParamInput,, ManufacturersReturned)			

	Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)

	Set Cmd = Nothing
	Call CloseSQLConnection(ctReadWrite)

end function
'****************************************************************************************************************************************************
' Returns a space delimeted list of cleansed and analyzed keywords
'
' Keywords - user input
' OnlyONeCharKeywords - To indicate if user only typed in OneCharKeyWords
'****************************************************************************************************************************************************
function keyWordAnalysis(Keywords, byRef OnlyOneCharKeywords)

	dim tempKeywords, KeyWordArray, Temp, Temp1, OutputWord, AliasWord, tempKeyword
	dim Count, WordCount, WordsGreaterThanOneChar

	tempKeywords = trim(Keywords)

	'' remove troublesome characters from the keyword list
	do while instr(tempKeywords, "  ") > 0
		tempKeywords = replace(tempKeywords, "  ", " ")
	loop
	tempKeywords = replace(tempKeywords,"'","")
	tempKeywords = replace(tempKeywords, chr(34),"") '' Double Quote
	tempKeywords = replace(tempKeywords, "*", "")
	tempKeywords = replace(tempKeywords, "+", "")
	'Added to fix the broken page
	tempKeywords = replace(tempKeywords, " )", "")
	tempKeywords = replace(tempKeywords, " (", "")
	tempKeywords = replace(tempKeywords, "( ", "")
	tempKeywords = replace(tempKeywords, " - ", "-")
	tempKeywords = replace(tempKeywords, " @", "")
	tempKeywords = replace(tempKeywords, " .", "")
	tempKeywords = replace(tempKeywords, " /", "")
	tempKeywords = replace(tempKeywords, ":", "")
	tempKeywords = replace(tempKeywords, "# ", "#")
	If Right(tempKeywords, 2) = " #" then
		tempKeywords = left(tempKeywords, len(tempKeywords) - 2)
	end if
	
	'Parse the words into an array
	KeywordArray = Split( tempKeywords, " " )

	'Loop through the Keywords one at a time
	For Count = LBOUND(KeywordArray) to UBOUND(KeywordArray)

		tempKeyWord = Trim(KeywordArray(Count))

		if Len(tempKeyWord) <> 0 then

			'' Clean the word
			Call AnalyzeWord(tempKeyWord, OutputWord, AliasWord)

			'' Outputword will be null or empty if the word is supposed to be ignored
			if len(OutputWord) > 0 then

				if len(tempKeyWord) = 1 then
					WordsGreaterThanOneChar = WordsGreaterThanOneChar + 1
				end if

				wordcount = wordcount + 1

				'Do we search for a word and it's alias?
				if len(AliasWord) > 0  then

					Temp = Temp & " [?al?] " & OutputWord & " " & AliasWord 

				else 'search for just the word

					Temp = Temp & " " & OutputWord  

				end if 

			end if

		end if 'Check for non zero length words

	next ' loop through Keywords


	if len(Temp) = 0 then
		Temp = ""
	end if

	'' If all words are one character long
	if 	WordsGreaterThanOneChar = wordcount then
		OnlyOneCharKeywords = true
	else
		OnlyOneCharKeywords = false
	end if

	keyWordAnalysis = Temp
end function
'****************************************************************************************************************************************************
Function ProcessKeywordsAndGenerateContainsSQL(Keywords, FulltextSearchColmns, useORInsteadOfAnd)

	dim FirstWordProcessed, tempKeywords, KeywordArray, Count, temp1, temp, TempKeyWord
	dim aliasWordCount, results, aliasFlag

	tempKeywords = Keywords
	FirstWordProcessed = false
	results = ""
	temp = ""

	'Parse the words into an array (they should be space delimeted)
	KeywordArray = Split( tempKeywords, " " )


	'Loop through the Keywords one at a time
	For Count = 0 to UBOUND(KeywordArray)

		tempKeyWord = KeywordArray(Count)

		if len(tempKeyWord) > 0 then 


			' Control sequences
			' [?al?] indicates the next two words are alias for one another
			If Left(tempKeyWord,6) = "[?al?]" Then 
				aliasFlag = true
				aliasWordCount = 0
			Else ' word is a normal word

				if ( FirstWordProcessed = true ) then 
					if ( aliasFlag = true and AliasWordCount = 0 ) OR ( aliasFlag = false ) then
						if UseOrInsteadOfAnd then
							Temp = Temp & " OR "
						else
							Temp = Temp & " AND "
						end if
					end if
				end if

				' Output the first paren to start the alias word pair ( word1 or word2 )
				if (aliasFlag = true) and (aliasWordCount = 0) then
					Temp = Temp & " ( "
				end if

				' Output the word
				On Error Resume Next
				dim tempKeyWordInt
				tempKeyWordInt = CInt(tempKeyWord)
				If Err.Number <> 0 Then
					Temp = Temp & Chr(34) & tempKeyWord & "*" & Chr(34)
					Err.Number = 0
				Else
					Temp = Temp & Chr(34) & tempKeyWord & Chr(34)
				End If

				' Use the or to include the second alias word in the string
				if (aliasFlag = true) and (aliasWordCount = 0) then
					Temp = Temp & " OR "
				end if

				' Output the second paren to end the alias word pair   ( word1 or word2 )
				if (aliasFlag = true) and (aliasWordCount = 1) then
					Temp = Temp & " ) "
				end if

				' Reset alias variables for next word. Did we deal with the first or second alias word?
				if (aliasFlag = true) and aliasWordCount = 0 then
					aliasWordCount = 1
				elseif (aliasFlag = true) and aliasWordCount = 1 then
					aliasFlag = false
				end if

				FirstWordProcessed = true
			end if
		end if 
	next ' loop through Keywords

	if len(Temp) > 0 then
		Temp = " CONTAINS(" & FulltextSearchColmns &  " , '" & Temp & "')"
	else 
		Temp = ""
	end if

	ProcessKeywordsAndGenerateContainsSQL = Temp
End Function
%>
