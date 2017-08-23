<%
Function AddOrderItemBeacon(OrderID)
	Dim TrackCookie, SessionCookie, UserKey
	Dim returnValue
	Dim oCmd
	returnValue = Null
	TrackCookie = GetTrackKey()
	SessionCookie = GetSessionKey()
	UserKey = GetUserKey()

	'Response.Write(TrackCookie & "<br/>")
	'Response.Write(SessionCookie & "<br/>")
	'Response.Write(UserKey & "<br/>")
	'Response.End()

	If Not(IsNull(OrderID)) and Not(IsEmpty(OrderID)) and IsNumeric(OrderID) Then
		Call OpenSQLConnection(ctReadWrite)
	    Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.newCheckout_AddOrderItemBeacon")
		oCmd.Parameters.Append oCmd.CreateParameter ("@OrderID",adInteger,adParamInput,,OrderID)
		oCmd.Parameters.Append oCmd.CreateParameter ("@TrackCookie",adVarChar, adParamInput,32,TrackCookie)
		oCmd.Parameters.Append oCmd.CreateParameter ("@SessionCookie",adVarChar, adParamInput,32,SessionCookie)
		oCmd.Parameters.Append oCmd.CreateParameter ("@UserKey",adVarChar, adParamInput,32,UserKey)
		Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		returnValue = oCmd.Parameters(0).Value
		Set oCmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
		
	AddOrderItemBeacon = returnValue
End Function

Function AddCartItemBeacon(CartItemID)
	Dim TrackCookie, SessionCookie, UserKey
	Dim returnValue
	Dim oCmd
	returnValue = Null
	TrackCookie = GetTrackKey()
	SessionCookie = GetSessionKey()
	UserKey = GetUserKey()

	If Not(IsNull(CartItemID)) and Not(IsEmpty(CartItemID)) and IsNumeric(CartItemID) Then
		Call OpenSQLConnection(ctReadWrite)
	    Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.ShoppingCart_AddCartItemBeacon")
		oCmd.Parameters.Append oCmd.CreateParameter ("@CartItemID",adInteger,adParamInput,,CartItemID)
		oCmd.Parameters.Append oCmd.CreateParameter ("@TrackCookie",adVarChar, adParamInput,32,TrackCookie)
		oCmd.Parameters.Append oCmd.CreateParameter ("@SessionCookie",adVarChar, adParamInput,32,SessionCookie)
		oCmd.Parameters.Append oCmd.CreateParameter ("@UserKey",adVarChar, adParamInput,32,UserKey)
		Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		returnValue = oCmd.Parameters(0).Value
		Set oCmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
		
	AddCartItemBeacon = returnValue
End Function

Function DeleteCartItemBeacon(CartItemID)
	Dim returnValue
	Dim oCmd
	returnValue = Null

	If Not(IsNull(CartItemID)) and Not(IsEmpty(CartItemID)) and IsNumeric(CartItemID) Then
		Call OpenSQLConnection(ctReadWrite)
	    Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.ShoppingCart_DeleteCartItemBeacon")
		oCmd.Parameters.Append oCmd.CreateParameter ("@CartItemID",adInteger,adParamInput,,CartItemID)
		Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
		returnValue = oCmd.Parameters(0).Value
		Set oCmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
		
	DeleteCartItemBeacon = returnValue
End Function
%>