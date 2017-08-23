<%
' Modify History:
' Paul Haan (5/24/04):  Added Several Errors for Defect # 3156.

' Yang Sun  May 21, 2004. 
' For Defect #3141 : PAS Authorizer group no longer listed. 
'	add erPASGroupUnEditable
'   add SubErrors(erPASGroupUnEditable)
'******************************************************************************************************************************
	Dim MainErrors(7,1), SubErrors(9999), StoreErrors, StoreSubErrors(15)

	Const errInvalid		= 0
	Const errMissing		= 1
	Const errGeneral		= 2
	Const errSearch			= 3
	Const errGeneralSpanish	= 4
	Const errInvalidSpanish	= 5
	Const errSiteTransfer	= 6
	Const errSessionTimeout	= 7

	' All possible MAIN errors:
	MainErrors(errInvalid,0)	= "Invalid Request"
	MainErrors(errInvalid,1)	= "Your request is invalid."
	MainErrors(errMissing,0)	= "Missing Information"
	MainErrors(errMissing,1)	= "Your request is missing information."
	MainErrors(errGeneral,0)	= "General Error"
	MainErrors(errGeneral,1)	= "There was an error and your request was not processed."
	MainErrors(errSearch,0)		= "Search Error"
	MainErrors(errSearch,1) 	= "We did not find any products matching your request."
	MainErrors(errGeneralSpanish,0)	= "Error General"
	MainErrors(errGeneralSpanish,1)	= "Se detect&#243; un error y su encuesta no fue procesada"		'There was an error and your request was not processed.
	MainErrors(errInvalidSpanish,0)	= "Petici\u243n inv\u225lida"									'Invalid Request
	MainErrors(errInvalidSpanish,1)	= "Su petici&#243;n no es v&#225;lida.  Por favor corrija los siguientes datos:"	'Your request is invalid.
	MainErrors(errSiteTransfer,0)	= "Site Transfer"
	MainErrors(errSiteTransfer,1)	= "In order to serve you better, we have transferred you to another of our Web sites." &_
		"  Please read below for additional information."
	MainErrors(errSessionTimeout,0)	= "Your session has expired"
	MainErrors(errSessionTimeout,1)	= "Please log on to resume your session.<br /><br />Your session has expired due to an extended period of inactivity. Please re-enter your user name and password to resume your session."

'The errors have been broken down into categories.  
'The breakdown is as follows:
	'ERROR TYPE			 		  NUMBERS  
	'EACCOUNT ERRORS:			   0 - 999 
	'CONTACT ERRORS:			1000 - 1999
	'ADDRESS ERRORS:	 		2000 - 2999
	'PAYMENT/CHECKOUT/QUOTE/			   
	'	ORDERSTATUS ERRORS:		3000 - 3999
	'ASSET ERRORS:				4000 - 4999
	'EPP ERRORS:				5000 - 5999
	'PAS ERRORS:				6000 - 6999
	'MISC ERRORS:		 		7000 - 8999:  8000 starts the spanish errors.
	'LEASE ERRORS:		 		9000 - 9999

	'EACCOUNT ERRORS:					   				0 - 999
	const erEaccountUserDisabled						= 0
	const erEaccountLogOnInvalid						= 1
	const erEaccountPasswordAuthenticationInvalid		= 2
	const erEaccountUserNameInvalid						= 3
	const erEaccountUserNameExists						= 4
	const erEaccountUserNameMatchesPassword				= 5
	const erEaccountPasswordInvalid						= 6
	const erEaccountPasswordMissMatch					= 7
	const erEaccountVerifyPasswordEmpty					= 8
	const erEaccountHintQuestionEmpty					= 9
	const erEaccountHintAnswerEmpty						= 10
	const erEaccountCustomerIDInvalid					= 11
	const erEaccountOrderNumberInvalid					= 12
	const erEaccountExtranetInvalid						= 13
	const erEaccountDidNotAgree							= 14
	const erEaccountWrongGSite							= 15
	const erEaccountOrganizationNameInvalid				= 16
	const erEaccountRequestInvalid						= 17
	const erEaccountLogoURLInvalid						= 18
	const erEaccountHintAnswerInvalid					= 19
	const erUserDoesNotBelongToOrganization				= 20
	const erEaccountBadUserKey							= 21
	const erEaccountNoUserIDForUserKey					= 22
	const erEaccountNoOrgIDForUserID					= 23
	const erEaccountNoDataForOrganizationID				= 24
	const erEaccountNoCustomerIDForOrganizationCode		= 25
	const erEaccountUserNotLoggedOn						= 26
	const erEaccountAlreadyLinked						= 27
	const erEaccountNotSetUpForPreview					= 28
	const erEaccountPurchaseTypeEmpty					= 29
	const erEaccountPurchaseTypeInvalidForCurrentSite	= 30
	const erEaccountEmailLengthInvalid                  = 31
	const erEaccountCustomerNumberInvalid               = 32
	const erEaccountUserLocked							= 33

	'CONTACT ERRORS:					 				1000 - 1999
	const erFirstNameInvalid							= 1000
	const erLastNameInvalid								= 1001
	const erFullNameInvalid								= 1002
	const erTitleEmpty								= 1003
	const erPhoneInvalid								= 1004
	const erInternationalPhoneInvalid						= 1005
	const erPhoneExtInvalid								= 1006
	const erFaxInvalid								= 1007
	const erEmailEmpty								= 1008
	const erEmailInvalid								= 1009
	const erOrganizationPhoneInvalid						= 1010
	const erOrganizationInternationalPhoneInvalid					= 1011
	const erToEmailEmpty								= 1012
	const erToEmailInvalid								= 1013
	const erFromEmailEmpty								= 1014
	const erFromEmailInvalid							= 1015
	const erContactName								= 1016
	const ervEmailInvalid								= 1017
	const erCompanyNameEmpty                            = 1018
	const erSubjectInvalid							= 1019
	

	'ADDRESS ERRORS:	 				 			2000 - 2999
	const erInvalidAddress								= 2000
	const erShipToNameEmpty								= 2001
	const erBillToNameEmpty								= 2002
	const erAddress1Empty								= 2003
	const erCityEmpty									= 2004
	const erStateInvalid								= 2005
	const erProvinceInvalid								= 2006
	const erZipInvalid								= 2007
	const erPostalCodeInvalid							= 2008
	const erCountryInvalid								= 2009
	const erAttentionToInvalid							= 2010
	const erAttentionToEmailEmpty							= 2011
	const erSaveShippingNotSelected							= 2012
	const erAddressChangeReasonInvalid						= 2013
	const erAddressChangeOldAddressInvalid						= 2014
	const erPostalZipInvalid							= 2015
	const erAddress2Invalid								= 2016
	const erBillingAddressTypeInvalid						= 2017
	const erPostalCodeCanadaInvalid						= 2018
	const erZipExtInvalid								= 2019
	const erAddress1LengthRestriction					= 2020
	const erAddress2LengthRestriction					= 2021


	'PAYMENT/CHECKOUT/ORDER/QUOTE ERRORS: 				3000 - 3999
	const erPaymentInvalid								= 3000
	const erPaymentNameEmpty							= 3001
	const erPaymentPaymentTypeEmpty						= 3002
	const erPaymentPaymentTypeInvalid					= 3003
	const erPaymentCreditCardNumberEmpty				= 3004
	const erPaymentCreditCardNumberInvalid				= 3005
	const erPaymentCreditCardExpireDateInvalid			= 3006
	const erPaymentLeaseInvalid							= 3007
	const erpaymentInvalidValidationSequence			= 3008
	const erpaymentNoMethodSelected						= 3009
	const erpaymentNoLocationSelected					= 3010
	const erpaymentNameInvalid							= 3011
	const erpaymentPONumberInvalid						= 3012
	const erpaymentCostCenterInvalid					= 3013
	const erDescriptionInvalid							= 3014
	const erCheckoutItemNotTransferredToOrder			= 3015
	const erCheckoutCartItemInvalid						= 3016
	const erCheckoutCartPriceInvalid					= 3017
	const erCheckoutCartProductPurchasable				= 3018
	const erCheckoutCartCantPlaceOrders					= 3019
	const erCheckoutCartWaitingForApproval				= 3020
	const erCheckoutCartRejected						= 3021
	const erCheckoutCartUserNotAuthorizedToPurchase		= 3022
	const erCheckoutCartMonetaryLimitExceeded			= 3023
	const erCheckoutCartAddBundleError					= 3024
	const erCheckoutCantPlaceOrders						= 3025
	const erCheckoutWaitingForApproval					= 3026
	const erCheckoutRejected							= 3027
	const erCheckoutUserNotAuthorizedToPurchase			= 3028
	const erShippingMethodInvalidFedEx					= 3029
	const erShippingMethodErrorSavingDefault			= 3030
	const erShippingMethodInvalidAirborne				= 3031
	const erShippingMethodInvalidDHL					= 3032
	const erShippingMethodInvalidUPS					= 3033
	const erShippingMethodInvalidExpress				= 3034
	const erSavedCartAppendError						= 3035
	const erSavedCartGeneralError						= 3036
	const erSavedCartRetrieveError						= 3037
	const erOrderNumberInvalid							= 3038
	const erInvoiceNumberInvalid						= 3039
	const erCheckoutPOEmpty								= 3040
	const erRMANoProductChoosen							= 3041
	const erRMANoReasonGiven							= 3042
	const erRMAReasonGivenInvalid						= 3043
	const erQuoteInvalid								= 3044
	const erQuoteDetailMissing							= 3045
	const erQuoteDoesNotBelongToOrganization			= 3046
	const erOrderIDerInvalid							= 3047
	const erGeneralOrderUploadError						= 3048
	const erSavingOrderShippingAddress					= 3049
	const erShippingAddressIDInvalid					= 3050
	const erWillCallInvalid								= 3051
	const erCartSubTotal								= 3052
	const erBundleError									= 3053
	const erSavedCartDeleteError						= 3054
	const erSavedCartDeleteItemError					= 3055
	const erSavedCartKeyError							= 3056
	const erSavedCartNameError							= 3057
	const erSavedCartSaveError							= 3058
	const erEmailQuoteDoesNotBelongToOrganization		= 3059
	const erEmailQuoteInvalid							= 3060
	const erQuoteInCart									= 3061
	const erProductNotValid								= 3062
	const erCheckoutContractPriceInvalid				= 3063
	const erQuoteIsClosed								= 3064
	const erCachingQuote								= 3065
	const erInsertingOrderFees							= 3066
	const erDeletingOrderFees							= 3067
	const erUsersCompanyCantCheckout					= 3068
	const erQuantityInvalid								= 3069
	const erShippingMethodNotFound						= 3070
	const erBillAddressID								= 3071
	const erCheckoutNoBillingAddressFound				= 3072
	const erCheckoutSavingBillingAddress				= 3073
	const erCheckoutGeneralPaymentMethodError			= 3074
	const erCheckoutSavingPaymentMethodError			= 3075
	const erQuoteOnlyPage								= 3076
	const erQuoteReferenceMissing						= 3077
	const erBStockItemAlreadyInCart						= 3088
	const erSavedCartDuplicateNameError					= 3089	

	'ASSET ERRORS:										4000 - 4999
	const erAssetSearchCriteriaInvalid					= 4000
	const erAssetFieldNumberInvalid						= 4001
	const erAssetFieldNameInvalid						= 4002
	const erAssetFieldDescriptionInvalid				= 4003
	const erAssetTagIDInvalid							= 4004
	const erAssetCategoryIDInvalid						= 4005
	const erAssetProductNameInvalid						= 4006
	const erAssetSerialNumberInvalid					= 4007
	const erAssetManufacturerInvalid					= 4008
	const erAssetSoftwareRevisionInvalid				= 4009
	const erAssetCreatedByInvalid						= 4010
	const erAssetQCedByInvalid							= 4011
	const erAssetLocationInvalid						= 4012
	const erAssetLineNumberInvalid						= 4013
	const erAssetLicenseAuthNumberInvalid				= 4014
	const erAssetLicenseAgreementNumberInvalid			= 4015
	const erAssetLicenseProductNameInvalid				= 4016
	const erAssetLicenseVersionInvalid					= 4017
	const erAssetLicenseNumberUserInvalid				= 4018
	const erAssetLicenseCopiesInvalid					= 4019
	const erAssetLicenseIssueDateInvalid				= 4020
	const erAssetLicenseThisDateInvalid					= 4021
	const erAssetLicenseLastDateInvalid					= 4022
	const erAssetLicenseStartDateInvalid				= 4023
	const erAssetLicenseEndDateInvalid					= 4024
	const erAssetLicenseSupportPlanFeeInvalid			= 4025
	const erAssetLicenseSupportInvoiceInvalid			= 4026
	const erAssetLicenseLicenseProgramInvalid			= 4027
	const erAssetLicensePurchaseTypeInvalid				= 4028
	const erAssetLicenseReferenceNumberInvalid			= 4029
	const erAssetLicenseProductNumberInvalid			= 4030
	const erAssetLicenseResellerInvalid					= 4031
	const erAssetLicenseeContactInvalid					= 4032
	const erAssetLicenseeNameInvalid					= 4033
	const erAssetFieldNotPartOfOrganization				= 4034
	const erAssetLicenseNotPartOfOrganization			= 4035
	const erAssetLicenseeNotPartOfLicense				= 4036

	'EPP ERRORS:										5000 - 5999
	const erEPPSQLError									= 5000
	const erEPPNoEPPLevelFound							= 5001
	const erEPPNoReferenceCode							= 5002
	const erEPPNoEPPIDFound								= 5003
	const erEPPNoAuthorizers							= 5004
	const erEPPNoAuthorizerOnEPP						= 5005
	const erEPPInvalidEPP								= 5006
	const erEPPReferenceCodeMissing						= 5007
	const erEPPReferenceCodeNotValid					= 5008
	const erEPPInvalidVerifyEmail						= 5009
	const erEPPEmailNotValidForEPP						= 5010
	const erEPPEmailNotUniqueForEPP						= 5011
	const erEPPDepartmentMissing						= 5012
	const erEPPNoAuthorizer								= 5013
	const erEPPNotValidEPP								= 5014
	const erEPPNotUniqueAuthorizer						= 5015
	const erEPPEmailAddressesWithSQLError				= 5016
	const erEPPEmailAddressesTooLong					= 5017
	const erEPPEmailAddressesInvalid					= 5018
	const erEPPDomainsInvalid							= 5019
	const erEPPInvalidInformation						= 5020

	'PAS ERRORS:										6000 - 6999
	const erPASMonetaryLimitMissing						= 6001
	const erPASUserNotInAdminGroup						= 6002
	const erPASGroupNameMissing							= 6003
	const erPASOrganizationDoesNotOwnGroup				= 6004
	const erPASMonetaryLimitNotNumeric					= 6005
	const erPASOrderHasBeenDeclined						= 6006
	const erPASOrderIsLocked							= 6007
	const erPASInvalidOverrideID						= 6008
	const erPASInvalidItemID							= 6009
	const erPASNoApprovalDueToPendingChanges			= 6010	
	const erPASGroupUnEditable							= 6011
	
	'MISC ERRORS:		 								7000 - 8999
	const erEDCInvalid									= 7000
	const erMiscServerError								= 7001
	const erBuyersEdgeChannelCodeExists					= 7002
	const erBuyersEdgeChannelCodeEmpty					= 7003
	const erBuyersEdgeNewsletterTooLong					= 7004
	const erDateFromInvalid								= 7005
	const erDateToInvalid								= 7006
	const erCatalogNameInvalid							= 7007
	const erContestEmailAddressNotUnique				= 7008
	const erContestStateNotEligible						= 7009
	const erToolsCatalogNoName							= 7010
	const erToolsCatalogNameInvalid						= 7011
	const erToolsEmailMissingAddress					= 7012
	const erPreviousSurveyResponse						= 7013
	const erContractInvalidError						= 7014
	const erMissingRequiredFields						= 7015
	const erSearchParametersInvalid						= 7016
	const erSearchNoResultsFound						= 7017
	const erSBBInvalidManufacturer						= 7018
	const erRebateInvalid								= 7019
	const erDonationsEventDate							= 7020
	const erDonationsDeadline							= 7021
	const erDonationsSendTo								= 7022
	const erDonationsBenefactorName						= 7023
	const erDonationsBenefactorContact					= 7024
	const erDonationsBenefactorAddress					= 7025
	const erDonationsNatureofEvent						= 7026
	const erDonationsRequesting							= 7027
	const erDonationsPayableTo							= 7028
	const erDonationsSpecialNotes						= 7029
	const erSearchDataError								= 7030
	const erDonationsPastEventDate						= 7031
	const erDonationsPastDeadline						= 7032
	const erCatalogTypeMissing							= 7033
	const erLinkNameTooLong								= 7034
	const erLinkURLTooLong								= 7035
	const erBundleNameTooLong							= 7036
	const erBundleDescriptionTooLong					= 7037
	const erComparisonChartNameTooLong					= 7038
	const erToolsCatalogNoDescription					= 7039
    const erInvalidMessageLength                        = 7040
	const erNoSoftwareType								= 7041
	const erBundleNameMissing							= 7042
	const erBundleNameInvalid							= 7043
	const erBundleDescriptionInvalid					= 7044

	'Spanish errors for Surveys
	const erspanishPreviousSurveyResponse				= 8000
	const erspanishEaccountCustomerIDInvalid			= 8001
	const erspanishEaccountOrganizationNameInvalid		= 8002
	const erspanishFirstNameInvalid						= 8003
	const erspanishLastNameInvalid						= 8004
	const erspanishAddress1Empty						= 8005
	const erspanishAddress2Invalid						= 8006
	const erspanishCityEmpty							= 8007
	const erspanishStateInvalid							= 8008
	const erspanishPostalZipInvalid						= 8009
	const erspanishPhoneInvalid							= 8010
	const erspanishPhoneExtInvalid						= 8011
	const erspanishFaxInvalid							= 8012
	const erspanishEmailInvalid							= 8013

	'LEASE ERRORS										9000-9999
	const erLeaseNoMatch								= 9000
	const erLeaseNoCompany								= 9001
	const erLeaseNoFedTaxID								= 9002
	const erLeaseNoAddress								= 9003
	const erLeaseNoCity									= 9004
	const erLeaseNoState								= 9005
	const erLeaseNoZip									= 9006
	const erLeaseNoFirstName							= 9007
	const erLeaseNoLastName								= 9008
	const erLeaseNoPhone								= 9009
	const erLeaseBadFedTaxID							= 9010
	const erLeaseBadZip									= 9011
	const erLeaseBadPhone								= 9012

'******************************************************************************************************************************
' SubError Messages:
	'Eaccount Errors:										0 - 999
	SubErrors(erEaccountUserDisabled)						= "Your account has been disabled please Contact your account manager for assistance."
	SubErrors(erEaccountUserLocked)							= "Your account has been locked please contact site support for assistance."	
	SubErrors(erEaccountLogOnInvalid)						= "We do not recognize your user name or password, please check your records and try again. If you reached this message in error while trying to access your CDW E-Account or extranet, please e-mail E-Account@web.cdw.com for assistance."
	SubErrors(erEaccountPasswordAuthenticationInvalid)		= "Your user password is not recognized.Note:password is case sensitive.Please try again.<br><br>For help troubleshooting logon issues, <a href='/webcontent/inside/help/sitehints.asp#trouble'>click here</a>"
	SubErrors(erEaccountUserNameInvalid)					= "Please provide a valid user name. Your user name must be a minimum of 4 characters and may contain letters, numbers and symbols (&, *, #, etc.), but not spaces."
	SubErrors(erEaccountUserNameExists)						= "User name in use by another user, please choose another and try again."
	SubErrors(erEaccountUserNameMatchesPassword)			= "Invalid user name\password combination.  For security purposes we ask that passwords be unrelated to user names."
	SubErrors(erEaccountPasswordInvalid)					= "Your password must be different from your user name and is case sensitive. Password must be of a minimum of 6 characters and must contain at least one letter and one number. Symbols (&, *, #, etc.) may be used, but not spaces. Please try again."
	SubErrors(erEaccountPasswordMissMatch)					= "The passwords entered do not match. Please try again"
	SubErrors(erEaccountVerifyPasswordEmpty)				= "Please verify your password."
	SubErrors(erEaccountHintQuestionEmpty)					= "Please select a secret question."
	SubErrors(erEaccountHintAnswerEmpty)					= "Please provide the answer to your secret question. May contain letters, numbers and symbols except 50 character maximum, not case sensitive."
	SubErrors(erEaccountCustomerIDInvalid)					= "Please provide a valid <Company> customer number"
	SubErrors(erEaccountOrderNumberInvalid)					= "The invoice number and customer number do not match. Contact your account manager for assistance."
	SubErrors(erEaccountExtranetInvalid)					= "You do not have a extranet."
	SubErrors(erEaccountDidNotAgree)						= "In order to view contract information, you must be affiliated with an agency that is qualified to purchase from this contract."
	SubErrors(erEaccountWrongGSite)							= "You are not authorized log into this account."
	SubErrors(erEaccountOrganizationNameInvalid)			= "Please provide a valid organization name."
	SubErrors(erEaccountRequestInvalid)						= "Your request contains some invalid characters:  ^ * _ ` ~ \ | { } [ ] > <"
	SubErrors(erEaccountLogoURLInvalid)						= "The URL you entered for the logo contains some invalid characters:  ^ * _ ` ~ \ | { } [ ] > <"
	SubErrors(erEaccountHintAnswerInvalid)					= "Please check the answer you provided for your question"
	SubErrors(erUserDoesNotBelongToOrganization)			= "This user is not a member of your organization."
	SubErrors(erEaccountBadUserKey)							= "User Key is missing."
	SubErrors(erEaccountNoUserIDForUserKey)					= "User Key does not resolve to a User ID."
	SubErrors(erEaccountNoOrgIDForUserID)					= "User ID is not associated with an Organization ID."
	SubErrors(erEaccountNoDataForOrganizationID)			= "Organization ID returns no Organization Data."
	SubErrors(erEaccountNoCustomerIDForOrganizationCode)	= "Organization Code returns no Customer ID."
	SubErrors(erEaccountUserNotLoggedOn)					= "There is no current EAccount logged in."
	SubErrors(erEaccountAlreadyLinked)						= "Your E-Account is already linked to an organization.  Please contact your account manager for assistance."
	SubErrors(erEaccountNotSetUpForPreview)					= "You are not authorized to log into this site."
	SubErrors(erEaccountPurchaseTypeEmpty)					= "Please select a category which best describes your purchasing needs."
	SubErrors(erEaccountPurchaseTypeInvalidForCurrentSite)	= "You have selected a business or organization type that is served by" &_
		" a different CDW Web site.  You have been redirected to the appropriate site to complete your registration." &_
		"  Please continue below.  We appreciate your interest in CDW and CDW&middot;G."
	SubErrors(erEaccountEmailLengthInvalid)                 = "The email has exceeded the maximum allowable length of 500 characters."
	SubErrors(erEaccountCustomerNumberInvalid)              = "Please provide a valid customer number."

	'Contact Information Errors:							1000 - 1999
	SubErrors(erFirstNameInvalid)							= "Please provide a first name."
	SubErrors(erLastNameInvalid)							= "Please provide a last name."
	SubErrors(erFullNameInvalid)							= "Please provide a valid name."
	SubErrors(erTitleEmpty)									= "Please provide a valid job title."
	SubErrors(erPhoneInvalid)								= "Please provide a valid US/Canadian phone number where you can be reached.  An example of a valid format is 444-444-4444."
	SubErrors(erInternationalPhoneInvalid)					= "Please provide a valid US/Canadian or International phone number where you can be reached.  An example of a valid format is 444-444-4444 or +44 (0) 4444 44444."
	SubErrors(erPhoneExtInvalid)							= "The phone extension field can only be numeric."
	SubErrors(erFaxInvalid)									= "Please enter a valid fax number.  An example of a valid format is (444) 444-4444."
	SubErrors(erEmailEmpty)									= "Please provide a valid e-mail address."
	SubErrors(erEmailInvalid)								= "Please provide a valid e-mail address.  An example of a valid format is john@doe.com"
	SubErrors(ervEmailInvalid)								= "E-mail addresses entered do not match. Please try again"
	SubErrors(erCompanyNameEmpty)							= "Please provide a company/organization name."
	SubErrors(erOrganizationPhoneInvalid)					= "Please provide a valid US/Canadian phone number for your organization.  An example of a valid format is 444-444-4444."
	SubErrors(erOrganizationInternationalPhoneInvalid)		= "Please provide a valid US/Canadian or international phone number for your organization.  An example of a valid format is 444-444-4444 or +44 (0) 4444 44444."
	SubErrors(erToEmailEmpty)								= "Please provide an e-mail address for the recipient."
	SubErrors(erToEmailInvalid)								= "Please provide a valid e-mail address for the recipient.  An example of a valid format is john@doe.com"
	SubErrors(erFromEmailEmpty)								= "Please provide an e-mail address for the sender."
	SubErrors(erFromEmailInvalid)							= "Please provide a valid e-mail address for the sender.  An example of a valid format is john@doe.com"
	SubErrors(erContactName)								= "Please provide a valid contact name."
	SubErrors(erSubjectInvalid)								= "Please select a message subject."

	'Address Errors:										2000 - 2999
	SubErrors(erInvalidAddress)								= "Invalid address, please contact your account manager for assistance."
	SubErrors(erShipToNameEmpty)							= "Please provide a valid name for your shipping address."
	SubErrors(erBillToNameEmpty)							= "Please provide a valid name for your billing address."
	SubErrors(erAddress1Empty)								= "Please provide a valid address."
	SubErrors(erCityEmpty)									= "Please provide a city."
	SubErrors(erStateInvalid)								= "Please select a valid State for the United States."
	SubErrors(erProvinceInvalid)							= "Please select a valid Province for Canada."
	SubErrors(erZipInvalid)									= "Please provide a valid US Zip Code."
	SubErrors(erZipExtInvalid)								= "Please provide a valid US Zip Code Extension.  An example of a valid format is 1234."
	SubErrors(erPostalCodeInvalid)							= "Please provide a valid Canadian Postal Code."
	SubErrors(erCountryInvalid)								= "Please provide a valid country."
	SubErrors(erAttentionToInvalid)							= "Please provide an Attention to recipient."
	SubErrors(erAttentionToEmailEmpty)						= "Please provide an Attention to email."
	SubErrors(erSaveShippingNotSelected)					= "To set the address as your default shipping address, you must save it.  Please check ""Save This Shipping Address""."
	SubErrors(erAddressChangeReasonInvalid)					= "Your reason for change contains some invalid characters:  ^ * _ ` ~ \ | { } [ ] > <"
	SubErrors(erAddressChangeOldAddressInvalid)				= "Your old billing address contains some invalid characters:  ^ * _ ` ~ \ | { } [ ] > <"
	SubErrors(erPostalZipInvalid)							= "Please provide a valid zip code."
	SubErrors(erAddress2Invalid)							= "The secondary address information you entered is invalid."
	SubErrors(erBillingAddressTypeInvalid)							= "Please select a billing address type."
	SubErrors(erPostalCodeCanadaInvalid)							= "Please provide a valid postal code."
	SubErrors(erAddress1LengthRestriction)					= "Please provide the primary address which is not more than 30 characters."
	SubErrors(erAddress2LengthRestriction)					= "Please provide the secondary address which is not more than 30 characters."
	
	'Payment / checkout / orderstatus Errors				3000 - 3999
	SubErrors(erPaymentInvalid)								= "Invalid payment method.  Please Contact your Account Manager for assistance."
	SubErrors(erPaymentNameEmpty)							= "Please provide a valid name for your payment method."
	SubErrors(erPaymentPaymentTypeEmpty)					= "Please provide a payment type. If you prefer to phone in your payment information, please select Pay by Phone."
	SubErrors(erPaymentPaymentTypeInvalid)					= "Please provide a payment type."
	SubErrors(erPaymentCreditCardNumberEmpty)				= "Please provide a credit card number. If you do not have a valid credit card, please select Pay by Phone."
	SubErrors(erPaymentCreditCardNumberInvalid)				= "The credit card number you entered is not valid, please check the number."
	SubErrors(erPaymentCreditCardExpireDateInvalid)			= "The credit card number you entered has expired, please check the expiration date."
	SubErrors(erPaymentLeaseInvalid)						= "To lease, the purchase price must be over $2,000."
	SubErrors(erpaymentInvalidValidationSequence)			= "This credit card is not authorized by your institution.  Please contact your Account Manager for assistance."
	SubErrors(erpaymentNoMethodSelected)					= "You did not select a payment method for your order.  Please select a payment method."
	SubErrors(erpaymentNoLocationSelected)					= "Please select either Personal or Company payment method."
	SubErrors(erpaymentNameInvalid)							= "The name you entered contains invalid characters."
	SubErrors(erpaymentPONumberInvalid)						= "The PO Number you entered contains invalid characters."
	SubErrors(erpaymentCostCenterInvalid)					= "The Cost Center you entered contains invalid characters."
	SubErrors(erDescriptionInvalid)							= "The description you provided is invalid."
	SubErrors(erCheckoutItemNotTransferredToOrder)			= "Item not transferred to checkout Order."
	SubErrors(erCheckoutCartItemInvalid)					= "Product not available. Contact your Account Manager for assistance."
	SubErrors(erCheckoutCartPriceInvalid)					= "The Product has an invalid Price. Contact your Account Manager for assistance."
	SubErrors(erCheckoutCartProductPurchasable)				= "The Product can't be added to Your Cart."	
	SubErrors(erCheckoutCartCantPlaceOrders)				= "You are not authorized to purchase items. Contact your PAS Administrator."
	SubErrors(erCheckoutCartWaitingForApproval)				= "To purchase this Shopping Cart your company's EPP contact will have to authorize this order."
	SubErrors(erCheckoutCartRejected)						= "You are not authorized to checkout. Contact your Sales Administrator."
	SubErrors(erCheckoutCartUserNotAuthorizedToPurchase)	= "You are not authorized to checkout."
	SubErrors(erCheckoutCartMonetaryLimitExceeded)			= "You have exceeded your Monetary Limit."
	SubErrors(erCheckoutCartAddBundleError)					= "There was a problem adding your Bundle to this Cart. Please contact your Account Manager for assistance."
	SubErrors(erCheckoutCantPlaceOrders)					= "You are not authorized to purchase items. Contact Your PAS Administrator."
	SubErrors(erCheckoutWaitingForApproval)					= "To purchase this Shopping Cart, your company's EPP contact will have to authorize this order."
	SubErrors(erCheckoutRejected)							= "You are not authorized to checkout. Contact your Sales Administrator."
	SubErrors(erCheckoutUserNotAuthorizedToPurchase)		= "You are not authorized to checkout."
	SubErrors(erShippingMethodNotFound)						= "There was an error retrieving the shipping method you selected."
	SubErrors(erShippingMethodInvalidFedEx)					= "You have entered an invalid Federal Express Account Number."
	SubErrors(erShippingMethodErrorSavingDefault)			= "There was an error saving your default Shipping Method."
	SubErrors(erShippingMethodInvalidAirborne)				= "You have entered an invalid Airborne Account Number."
	SubErrors(erShippingMethodInvalidDHL)					= "You have entered an invalid DHL Account Number."
	SubErrors(erShippingMethodInvalidUPS)					= "You have entered an invalid UPS Account Number."
	SubErrors(erShippingMethodInvalidExpress)				= "You have entered an invalid Express Shipping Number."
	SubErrors(erSavedCartAppendError)						= "There was an error appending your current cart to the saved cart."
	SubErrors(erSavedCartGeneralError)						= "There was an error processing your request. Contact your Sales Rep for further Assistance."
	SubErrors(erSavedCartRetrieveError)						= "There was a problem retrieving your saved cart. Contact your Sales Rep for further Assistance."
	SubErrors(erSavedCartDeleteError)						= "There was an error deleting your cart."
	SubErrors(erSavedCartDeleteItemError)					= "There was an error deleting the item from your cart."
	SubErrors(erSavedCartKeyError)							= "There was an error generating a key for your saved cart."
	SubErrors(erSavedCartNameError)							= "Please provide a valid name for your saved cart."
	SubErrors(erSavedCartDuplicateNameError)				= "Cart name already exists. Please provide a different name."
	SubErrors(erSavedCartSaveError)							= "There was a problem saving your cart.  If you continue to receive this error, contact your Sales Rep for further Assistance."
	SubErrors(erCheckoutPOEmpty)							= "Please provide a Purchase Order Number. "
	SubErrors(erOrderNumberInvalid)							= "Please provide a valid order number."
	SubErrors(erInvoiceNumberInvalid)						= "Please provide a valid <Company> invoice number."
	SubErrors(erRMANoProductChoosen)						= "No products choosen"
	SubErrors(erRMANoReasonGiven)							= "Please enter the reason for the return."
	SubErrors(erRMAReasonGivenInvalid)						= "The reason for the return contains invalid characters:  ^ * _ ` ~ \ | { } [ ] > <"
	SubErrors(erQuoteInvalid)								= "The quote you are trying to view is no longer valid."
	SubErrors(erQuoteDetailMissing)							= "The detail lines for your quote could not be retrieved.  Please contact your Account Manager for assistance."
	SubErrors(erQuoteDoesNotBelongToOrganization)			= "The quote you are trying to view does not belong to your organization."
	SubErrors(erOrderIDerInvalid)							= "There was a problem getting your OrderID."
	SubErrors(erGeneralOrderUploadError)					= "There was a problem placing your Order. Please contact your account manager for assistance. Code:900"
	SubErrors(erGeneralOrderUploadError)					= "There was an error saving your saved shipping address with this Order"
	SubErrors(erShippingAddressIDInvalid)					= "There was a problem getting your shipping address. Please contact your Account Manager for assistance."
	SubErrors(erWillCallInvalid)							= "There was an error setting your order's shipping method -- Will Call."
	SubErrors(erCartSubTotal)								= "There was an error getting your cart sub-total"
	SubErrors(erBundleError)								= "There was a problem assigning your bundle."
	SubErrors(erEmailQuoteDoesNotBelongToOrganization)		= "The quote you are trying to send does not belong to your organization."
	SubErrors(erEmailQuoteInvalid)							= "The quote you are trying to send is no longer valid."
	SubErrors(erQuoteInCart)								= "You currently have a quote in your cart.  You can only purchase one quote at a time."
	SubErrors(erProductNotValid)							= "The EDC you entered is not valid.  Please try another EDC."
	SubErrors(erCheckoutContractPriceInvalid)				= "This item is not eligible for contract pricing as the applicable contract is no longer available. The item has been added to your cart at your organization's standard discounted price."
	SubErrors(erQuoteIsClosed)								= "This quote is closed and can not be purchased.  Please contact your Account Manager for assistance."
	SubErrors(erCachingQuote)								= "There was a problem adding your quote to the cart.  Please contact your Account Manager for assistance."
	SubErrors(erInsertingOrderFees)							= "There was a problem processing the contract fee for your order.  Please contact your Account Manager for assistance."
	SubErrors(erDeletingOrderFees)							= "There was a problem clearing the contract fee for the order.  Please contact your Account Manager for assistance."
	SubErrors(erUsersCompanyCantCheckout)					= "There is an issue with your companies account status. Please contact your account representitive before placing this order.<br><a href='/shop/eaccount/accountteam.asp' class='btslink'><b>Contact your Account Team</b> </a><span style='color:CC0000'><b>&#187;</b>"
	SubErrors(erQuantityInvalid)							= "The quantity you entered is invalid.  Please enter only numeric values for quantity."
	SubErrors(erBillAddressID)								= "Please select the Billing Address to use for your order."
	SubErrors(erCheckoutNoBillingAddressFound)				= "Unable to find a billing address for this order.  Please contact your Account Manager for assistance."
	SubErrors(erCheckoutSavingBillingAddress)				= "There was an error saving your billing address.  Please contact your Account Manager for assistance."
	SubErrors(erCheckoutGeneralPaymentMethodError)			= "There is an error with your payment method.  Please Contact your Account Manager for assistance."
	SubErrors(erCheckoutSavingPaymentMethodError)			= "There was an error saving your payment method.  Please Contact your Account Manager for assistance."
	SubErrors(erQuoteOnlyPage)								= "Please Contact your Account Manager for assistance in creating a quote."
	SubErrors(erQuoteReferenceMissing)						= "Please enter a Quote Reference."
	SubErrors(erBStockItemAlreadyInCart)					= "This is a clearance item.  Only a single unit may be added to your cart."

	'Asset Errors											4000 - 4999
	SubErrors(erAssetSearchCriteriaInvalid)					= "The search criteria is invalid."
	SubErrors(erAssetFieldNumberInvalid)					= "The number field is invalid.  Please input an numeric value only."
	SubErrors(erAssetFieldNameInvalid)						= "One of the field names is invalid.<br /> - Field names may not contain the following characters: ^ * _ ` ~ \ | { } [ ] > <<br /><br />"
	SubErrors(erAssetFieldDescriptionInvalid)				= "One of the field descriptions is invalid.<br /> - Field descriptions may not contain the following characters: ^ * _ ` ~ \ | { } [ ] > <"
	SubErrors(erAssetTagIDInvalid)							= "Please provide a valid asset tag ID is invalid."
	SubErrors(erAssetCategoryIDInvalid)						= "Please provide a valid asset category."
	SubErrors(erAssetProductNameInvalid)					= "Please provide a valid product name."
	SubErrors(erAssetSerialNumberInvalid)					= "Please provide a valid serial number."
	SubErrors(erAssetManufacturerInvalid)					= "Please provide a valid manufacturer."
	SubErrors(erAssetSoftwareRevisionInvalid)				= "Please provide a valid software revision."
	SubErrors(erAssetCreatedByInvalid)						= "The Created by field is invalid."
	SubErrors(erAssetQCedByInvalid)							= "The QCed by field is invalid."
	SubErrors(erAssetLocationInvalid)						= "The Location field is invalid."
	SubErrors(erAssetLineNumberInvalid)						= "Please provide a valid line number."
	SubErrors(erAssetLicenseAuthNumberInvalid)				= "Please provide a valid authorization number."
	SubErrors(erAssetLicenseAgreementNumberInvalid)			= "Please provide a valid agreement number."
	SubErrors(erAssetLicenseProductNameInvalid)				= "Please provide a valid Product name."
	SubErrors(erAssetLicenseVersionInvalid)					= "Please provide a valid version."
	SubErrors(erAssetLicenseNumberUserInvalid)				= "Please provide the valid number of users (numeric only)."
	SubErrors(erAssetLicenseCopiesInvalid)					= "Please provide the number copies (numeric only)."
	SubErrors(erAssetLicenseIssueDateInvalid)				= "Please provide a valid issue date."
	SubErrors(erAssetLicenseThisDateInvalid)				= "Please provide a valid issue date."
	SubErrors(erAssetLicenseLastDateInvalid)				= "Please provide a valid expiration date."
	SubErrors(erAssetLicenseStartDateInvalid)				= "Please provide a valid start date."
	SubErrors(erAssetLicenseEndDateInvalid)					= "Please provide a valid end date."
	SubErrors(erAssetLicenseSupportPlanFeeInvalid)			= "Please provide a valid support plan fee."
	SubErrors(erAssetLicenseSupportInvoiceInvalid)			= "Please provide a valid Support Invoice #."
	SubErrors(erAssetLicenseLicenseProgramInvalid)			= "Please provide a valid License Program."
	SubErrors(erAssetLicensePurchaseTypeInvalid)			= "Please provide a valid Purchase Type."
	SubErrors(erAssetLicenseReferenceNumberInvalid)			= "Please provide a valid Reference number."
	SubErrors(erAssetLicenseProductNumberInvalid)			= "Please provide a valid Product number."
	SubErrors(erAssetLicenseResellerInvalid)				= "Please provide a valid Reseller."
	SubErrors(erAssetLicenseeContactInvalid)				= "Please provide a valid licensee contact."
	SubErrors(erAssetLicenseeNameInvalid)					= "Please provide a valid licensee name."
	SubErrors(erAssetFieldNotPartOfOrganization)			= "This asset field specified is invalid. Please contact your adminstrator."
	SubErrors(erAssetLicenseNotPartOfOrganization)			= "This License specified is invalid. Please contact your adminstrator."
	SubErrors(erAssetLicenseeNotPartOfLicense)				= "This Licensee specified is invalid. Please contact your adminstrator."

	'EPP Errors												5000 - 5999
	SubErrors(erEPPSQLError)								= "There was a problem processing your request.  Please try again or contact a CDW account manager via phone or e-mail at <a href=""mailto:sales@web.cdw.com"">sales@web.cdw.com.</a>  We apologize for the inconvenience."
	SubErrors(erEPPNoEPPLevelFound)							= "Your Employee Purchase Program has not been activated. To continue you will need to <a href=""http:\\www.cdw.com/extranet/extranetCustomize.asp"">activate</a> your program."
	SubErrors(erEPPNoReferenceCode)							= "An error has occurred which prevents us from setting up your Employee Purchase Program right now.  Please contact your CDW account manager to address this problem and finish setting up your Employee Purchase Program." 
	SubErrors(erEPPNoEPPIDFound)							= "Your Employee Purchase Program has not been activated.  To continue you will need to <a href=""http:\\www.cdw.com/extranet/extranetCustomize.asp"">activate</a> your program."
	SubErrors(erEPPNoAuthorizers)							= "Your employer currently has no EPP Contacts listed.  Please contact a CDW account manager or your employer to have this issue resolved.  You will not be able to set up your EPP E-Account until an EPP Contact is listed."
	SubErrors(erEPPNoAuthorizerOnEPP)						= "Your employer currently has no EPP Contacts listed.  Please contact a CDW account manager or your employer to have this issue resolved.  You will not be able to set up your EPP E-Account until an EPP Contact is listed."
	SubErrors(erEPPInvalidEPP)								= "An error has occurred which prevents us from setting up your Employee Purchase Program right now.  Please contact your CDW account manager to address this problem and finish setting up your Employee Purchase Program." 
	SubErrors(erEPPReferenceCodeMissing)					= "Please enter an EPP Access Code.  This code is provided by your employer."
	SubErrors(erEPPReferenceCodeNotValid)					= "The EPP Access Code you entered is not valid.  Please try again or check with your employer to make sure you have the correct EPP Access Code."
	SubErrors(erEPPInvalidVerifyEmail)						= "Please provide an e-mail address that has been approved by your employer.  An example of a valid format is john@doe.com."
	SubErrors(erEPPEmailNotValidForEPP)						= "The e-mail address you listed as approved by your employer is not recognized as an approved address.  Please contact your employer or a CDW account manager."
	SubErrors(erEPPEmailNotUniqueForEPP)					= "This e-mail address already has an EPP E-Account created.  If you forgot your user name and/or password click on the link below and we will send the information to the approved e-mail address you just entered."
	SubErrors(erEPPDepartmentMissing)						= "Please enter a valid department."
	SubErrors(erEPPNoAuthorizer)							= "Please select an authorizer."
	SubErrors(erEPPNotValidEPP)								= "An error has occurred which prevents us from setting up your Employee Purchase Program right now.  Please contact your CDW account manager to address this problem and finish setting up your Employee Purchase Program." 
	SubErrors(erEPPNotUniqueAuthorizer)						= "This EPP Contact's e-mail address is already listed in you EPP contacts list."
	SubErrors(erEPPEmailAddressesWithSQLError)				= "There was a problem processing your request and the following e-mail address(es) could not be added.  Please try entering the e-mail address(es) again."
	SubErrors(erEPPEmailAddressesTooLong)					= "The following e-mail address(es) could not be inserted because they are too long.  E-mail addresses must be 75 characters or less."
	SubErrors(erEPPEmailAddressesInvalid)					= "The some of the e-mail addresses could not be added because they are not valid.  An example of a valid format is john@doe.com."
	SubErrors(erEPPDomainsInvalid)							= "The domain could not be inserted because it is invalid.  <B>CDW.COM</B> is an example of a valid domain."
	SubErrors(erEPPInvalidInformation)						= "The additional information you entered contains invalid characters:  ^ * _ ` ~ \ | { } [ ] > <  <br />"

	'PAS Errors												6000 - 6999
	SubErrors(erPASMonetaryLimitMissing)					= "Please type a dollar amount."
	SubErrors(erPASUserNotInAdminGroup)						= "You are not a member of the Authorizer or Administrator group.  You must be in one of these groups to view orders."
	SubErrors(erPASGroupNameMissing)						= "Please provide a valid group name."
	SubErrors(erPASOrganizationDoesNotOwnGroup)				= "The group selected is not for your Organization.  Please contact your account manager for assistance."
	SubErrors(erPASMonetaryLimitNotNumeric)					= "Please provide a valid dollar amount (numeric only)."
	SubErrors(erPASOrderHasBeenDeclined)					= "This order has been declined."
	SubErrors(erPASOrderIsLocked)							= "This order has been locked due to pending changes made by another user."
	SubErrors(erPASInvalidOverrideID)						= "Invalid pending change specified."
	SubErrors(erPASInvalidItemID)							= "Invalid line item specified."
	SubErrors(erPASNoApprovalDueToPendingChanges)			= "This order can not be approved/declined at this time due to pending changes."	
	SubErrors(erPASGroupUnEditable)							= "The PAS Authorizer Group cannot be edited."

	'Miscellaneous Errors									7000 - 8999
	SubErrors(erEDCInvalid)									= "Please provide a valid EDC."
	SubErrors(erMiscServerError)							= "Internal Server Error... Please try again."
	SubErrors(erBuyersEdgeChannelCodeExists)				= "The Channel Code that was entered already exists as another channel."
	SubErrors(erBuyersEdgeChannelCodeEmpty)					= "The Channel Code cannot be empty."
	SubErrors(erBuyersEdgeNewsletterTooLong)				= "The Maximum Length for a newsletter is 8000 Charecters."
	SubErrors(erDateFromInvalid)							= "Your from date selection is invalid.  Please choose a valid date."
	SubErrors(erDateToInvalid)								= "Your to date selection is invalid.  Please choose a valid date."
	SubErrors(erCatalogNameInvalid)							= "Please provide a name for your catalog."
	SubErrors(erContestEmailAddressNotUnique)				= "Your e-mail address has previously been entered in this contest."
	SubErrors(erContestStateNotEligible)					= "Residents of this state are not eligible for the contest."
	SubErrors(erToolsCatalogNoName)							= "Please enter a name."
	SubErrors(erToolsCatalogNameInvalid)					= "The name you entered contains invalid characters:    ^ * _ ` ~ \ | { } [ ] > <"
	SubErrors(erToolsEmailMissingAddress)					= "Please fill out both e-mail addresses."
	SubErrors(erPreviousSurveyResponse)						= "Our records indicate that you have already filled out this CDW survey."
	SubErrors(erContractInvalidError)						= "Please select a valid Contract."
	SubErrors(erMissingRequiredFields)						= "One or more required fields are invalid.  Please correct the information and resubmit the form."
	SubErrors(erSearchParametersInvalid)					= "Your search was not valid.  Please redefine your search"
	SubErrors(erSearchNoResultsFound)						= "No results matched your query.  Please redefine your search"
	SubErrors(erSBBInvalidManufacturer)						= "The manufacturer you selected is invalid.  Please select another manufacturer."
	SubErrors(erRebateInvalid)								= "Rebate not found.  Please select from the list of rebates below."
	SubErrors(erDonationsEventDate)							= "The event date you entered is not valid.  Please format the date mm/dd/yyyy"
	SubErrors(erDonationsDeadline)							= "The deadline you entered is not valid.  Please format the date mm/dd/yyyy"
	SubErrors(erDonationsSendTo)							= "Please select who to send the donation to."
	SubErrors(erDonationsBenefactorName)					= "Please provide a valid Benefactor name"
	SubErrors(erDonationsBenefactorContact)					= "Please provide a valid Benefactor contact"
	SubErrors(erDonationsBenefactorAddress)					= "Please provide a valid Benefactor address"
	SubErrors(erDonationsNatureofEvent)						= "The Nature of Event contains invalid character(s)."
	SubErrors(erDonationsRequesting)						= "Please provide a valid request for your donation."
	SubErrors(erDonationsPayableTo)							= "There are invalid characters in the ""make check payable to"" field."
	SubErrors(erDonationsSpecialNotes)						= "The Special Notes section for your request contains invalid character(s)."
	SubErrors(erSearchDataError)							= "There was an error retrieving your search results.  Please redefine your search."
	SubErrors(erDonationsPastEventDate)						= "The event date you entered is in the past.  Please enter a date in the future (mm/dd/yyyy)"
	SubErrors(erDonationsPastDeadline)						= "The deadline you entered is in the past.  Please enter a date in the future (mm/dd/yyyy)"
	SubErrors(erCatalogTypeMissing)							= "Please select a catalog from the available list."
	SubErrors(erLinkNameTooLong)							= "Shorten Link Name to 100 characters or less."
	SubErrors(erLinkURLTooLong)								= "Shorten Link URL to 1000 characters or less."
	SubErrors(erBundleNameTooLong)							= "Shorten Bundle Name to 75 characters or less."
	SubErrors(erBundleDescriptionTooLong)					= "Shorten Bundle Description to 150 characters or less."
	SubErrors(erComparisonChartNameTooLong)					= "Shorten Comparison Chart Name to 100 characters or less."
	SubErrors(erToolsCatalogNoDescription)					= "Please enter a description."
    SubErrors(erInvalidMessageLength)						= "Message Length is invalid."
	SubErrors(erNoSoftwareType)								= "Please select a software type."
	SubErrors(erBundleNameMissing)							= "Bundle Name is required."
	SubErrors(erBundleNameInvalid)							= "The Bundle Name you chose has some invalid characters.  Only A-Z, 0-9 and Spaces are permitted."
	SubErrors(erBundleDescriptionInvalid)					= "The Bundle Description you chose has some invalid characters.  Only A-Z, 0-9 and Spaces are permitted."

	'Spanish Error Message for the survey system
	SubErrors(erspanishPreviousSurveyResponse)				= "Nuestros registros indican que usted ya complet&#243; esta encuesta de CDW."
	SubErrors(erspanishEaccountCustomerIDInvalid)			= "N&#250;mero de cuenta inv&#225;lido."								'Invalid customer number
	SubErrors(erspanishEaccountOrganizationNameInvalid)		= "Por favor porporcione un nombre de organizaci&#243;n v&#225;lido."	'Please provide a valid organization name.
	SubErrors(erspanishFirstNameInvalid)					= "Por favor proporcione un nombre propio v&#225;lido."					'Please provide a valid first name.
	SubErrors(erspanishLastNameInvalid)						= "Por favor proporcione un apellido v&#225;lido."						'Please provide a valid last name.
	SubErrors(erspanishAddress1Empty)						= "Por favor proporcione una direcci&#243;n v&#225;lida."				'Please provide a valid address.
	SubErrors(erspanishAddress2Invalid)						= "La informaci&#243;n de su segunda direcci&#243;n es inv&#225;lida."	'The secondary address information you entered is invalid.
	SubErrors(erspanishCityEmpty)							= "Por favor proporcione una ciudad v&#225;lida."						'Please provide a city.
	SubErrors(erspanishStateInvalid)						= "Por favor seleccione un estado v&#225;lido de los Estados Unidos."	'Please select a valid State for the United States.
	SubErrors(erspanishPostalZipInvalid)					= "Por favor proporcione un c&#243;digo postal v&#225;lido."				'Please provide a valid zip code."
	SubErrors(erspanishPhoneInvalid)						= "Por favor proporcione un n&#250;mero de tel&#233;fono v&#225;lido en los Estados Unidos o Canad&#225; donde podamos ponernos en contacto con usted.  Un ejemplo de formato v&#225;lido es:  (444) 444-4444."	'Please provide a valid US/Canadian phone number where you can be reached.  An example of a valid format is (444) 444-4444."
	SubErrors(erspanishPhoneExtInvalid)						= "La casilla de extensi&#243;n telef&#243;nica solo puede ser llenada num&#233;ricamente."	'The phone extension field can only be numeric."
	SubErrors(erspanishFaxInvalid)							= "Por favor proporcione un n&#250;mero de fax v&#225;lido.  Un ejemplo de formato v&#225;lido es:  (444) 444-4444."	'Please enter a valid fax number.  An example of a valid format is (444) 444-4444."
	SubErrors(erspanishEmailInvalid)						= "Por favor proporcione una dirección de correo electr&#243;nico v&#225;lida.  Un ejemplo de formato v&#225;lido es:  john@doe.com"  'Please provide a valid e-mail address.  An example of a valid format is john@doe.com"

	'Lease Errors											9000 - 9999
	SubErrors(erLeaseNoMatch)								= "No Lease Applications Found."
	SubErrors(erLeaseNoCompany)								= "You must enter a Company Name."	
	SubErrors(erLeaseNoFedTaxID)							= "You must enter a Federal Tax ID Number."	
	SubErrors(erLeaseNoAddress)								= "You must enter an address."
	SubErrors(erLeaseNoCity)								= "You must enter a city."	
	SubErrors(erLeaseNoState)								= "You must enter a state."
	SubErrors(erLeaseNoZip)									= "You must enter a zip code."
	SubErrors(erLeaseNoFirstName)							= "You must enter a contact first name."
	SubErrors(erLeaseNoLastName)							= "You must enter a contact last name."
	SubErrors(erLeaseNoPhone)								= "You must enter a contact phone number."
	SubErrors(erLeaseBadFedTaxID)   						= "Please enter a valid Federal Tax ID Number for lessee."
	SubErrors(erLeaseBadZip)								= "Please enter a valid zip code for Lessee."
	SubErrors(erLeaseBadPhone)								= "Please enter a valid phone number for lessee. "

'***************************************************************
'	DESCRIPTION:
'		Returns a querystring fragment representing a error state
'		Returns String
Function GetErrorQueryString()
	Dim returnValue
	returnValue = ""
	If(IsNumeric(StoreErrors)) Then
		returnValue = returnValue & "err=" & StoreErrors
		If(IsArray(StoreSubErrors)) Then
			returnValue = returnValue & "&serr=" & Join(StoreSubErrors, ",")
		End If
	End If
	GetErrorQueryString = returnValue
End Function
'***************************************************************
'	DESCRIPTION:
'		Records a run time error for the page.
'		Returns True/False
Function ErrorRase(Number)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(Number)) and Not(IsNull(Number)) and IsNumeric(Number)) Then
		StoreErrors = Number
		glPageError = True
		returnValue = False
	End If
	ErrorRase = returnValue
End Function
'***************************************************************
'	DESCRIPTION:
'		Records a run time error in the error collection for the
'		page.
'		Returns True/False
Function ErrorRaseSub(Number)
	Dim returnValue, i
	If(Not(IsEmpty(Number)) and Not(IsNull(Number)) and IsNumeric(Number)) Then

		For i = LBound(StoreSubErrors) To UBound(StoreSubErrors)
			If(IsEmpty(StoreSubErrors(i)) or IsNull(StoreSubErrors(i)) or Not(IsNumeric(StoreSubErrors(i)))) Then
				StoreSubErrors(i) = Number
				Exit For
			End If
		Next

		glPageError = True
		returnValue = False
	End If
	ErrorRaseSub = returnValue
End Function
'***************************************************************
Function GetLabelColorBySubError(Number)
	Dim returnValue, SubErrorsString
	returnValue = null
	SubErrorsString = request("serr")
	If inStr(SubErrorsString, Number) > 0 Then
		returnValue = "Red"
	End If
	GetLabelColorBySubError = returnValue
End Function

'***************************************************************
Sub RequireNumericSecurityCheck(Variable, ErrorCode, ErrorCodeSub, ErrorRedirectPage)
	If Len( Variable & "" ) <= 0 Or ( Not IsNumeric( Variable ) ) Then
		Call SecurityRedirect( ErrorCode, ErrorCodeSub, ErrorRedirectPage )
	End If
End Sub

'***************************************************************
Sub SecurityRedirect(ErrorCode, ErrorCodeSub, ErrorRedirectPage)
	ErrorRase( ErrorCode )
	ErrorRaseSub( ErrorCodeSub )
	Response.Redirect( ErrorRedirectPage & GetErrorQueryString() )
End Sub
%>
