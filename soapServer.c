/* soapServer.c
   Generated by gSOAP 2.8.122 for sclient.h

gSOAP XML Web services tools
Copyright (C) 2000-2021, Robert van Engelen, Genivia Inc. All Rights Reserved.
The soapcpp2 tool and its generated software are released under the GPL.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
--------------------------------------------------------------------------------
A commercial use license is available from Genivia Inc., contact@genivia.com
--------------------------------------------------------------------------------
*/

#if defined(__BORLANDC__)
#pragma option push -w-8060
#pragma option push -w-8004
#endif
#include "soapH.h"

SOAP_SOURCE_STAMP("@(#) soapServer.c ver 2.8.122 2022-05-30 15:28:18 GMT")
SOAP_FMAC5 int SOAP_FMAC6 soap_serve(struct soap *soap)
{
#ifndef WITH_FASTCGI
	soap->keep_alive = soap->max_keep_alive + 1;
#endif
	do
	{
#ifndef WITH_FASTCGI
		if (soap->keep_alive > 0 && soap->max_keep_alive > 0)
			soap->keep_alive--;
#endif
		if (soap_begin_serve(soap))
		{	if (soap->error >= SOAP_STOP)
				continue;
			return soap->error;
		}
		if ((soap_serve_request(soap) || (soap->fserveloop && soap->fserveloop(soap))) && soap->error && soap->error < SOAP_STOP)
		{
#ifdef WITH_FASTCGI
			soap_send_fault(soap);
#else
			return soap_send_fault(soap);
#endif
		}
#ifdef WITH_FASTCGI
		soap_destroy(soap);
		soap_end(soap);
	} while (1);
#else
	} while (soap->keep_alive);
#endif
	return SOAP_OK;
}

#ifndef WITH_NOSERVEREQUEST
SOAP_FMAC5 int SOAP_FMAC6 soap_serve_request(struct soap *soap)
{
	(void)soap_peek_element(soap);
	if (!soap_match_tag(soap, soap->tag, "ns:connect"))
		return soap_serve_ns__connect(soap);
	if (!soap_match_tag(soap, soap->tag, "ns:echo"))
		return soap_serve_ns__echo(soap);
	if (!soap_match_tag(soap, soap->tag, "ns:concat"))
		return soap_serve_ns__concat(soap);
	if (!soap_match_tag(soap, soap->tag, "ns:adder"))
		return soap_serve_ns__adder(soap);
	if (!soap_match_tag(soap, soap->tag, "ns:bye"))
		return soap_serve_ns__bye(soap);
	return soap->error = SOAP_NO_METHOD;
}
#endif

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_ns__connect(struct soap *soap)
{	struct ns__connect soap_tmp_ns__connect;
	struct ns__connectResponse soap_tmp_ns__connectResponse;
	long soap_tmp_ns__longType;
	soap_default_ns__connectResponse(soap, &soap_tmp_ns__connectResponse);
	soap_default_ns__longType(soap, &soap_tmp_ns__longType);
	soap_tmp_ns__connectResponse.connect = &soap_tmp_ns__longType;
	soap_default_ns__connect(soap, &soap_tmp_ns__connect);
	if (!soap_get_ns__connect(soap, &soap_tmp_ns__connect, "ns:connect", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = ns__connect(soap, soap_tmp_ns__connectResponse.connect);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL; /* use SOAP literal style */
	soap_serializeheader(soap);
	soap_serialize_ns__connectResponse(soap, &soap_tmp_ns__connectResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__connectResponse(soap, &soap_tmp_ns__connectResponse, "ns:connectResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__connectResponse(soap, &soap_tmp_ns__connectResponse, "ns:connectResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_ns__echo(struct soap *soap)
{	struct ns__echo soap_tmp_ns__echo;
	struct ns__echoResponse soap_tmp_ns__echoResponse;
	char * soap_tmp_ns__stringType;
	soap_default_ns__echoResponse(soap, &soap_tmp_ns__echoResponse);
	soap_tmp_ns__stringType = NULL;
	soap_tmp_ns__echoResponse.echo = &soap_tmp_ns__stringType;
	soap_default_ns__echo(soap, &soap_tmp_ns__echo);
	if (!soap_get_ns__echo(soap, &soap_tmp_ns__echo, "ns:echo", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = ns__echo(soap, soap_tmp_ns__echo.echoRequest, soap_tmp_ns__echoResponse.echo);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL; /* use SOAP literal style */
	soap_serializeheader(soap);
	soap_serialize_ns__echoResponse(soap, &soap_tmp_ns__echoResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__echoResponse(soap, &soap_tmp_ns__echoResponse, "ns:echoResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__echoResponse(soap, &soap_tmp_ns__echoResponse, "ns:echoResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_ns__concat(struct soap *soap)
{	struct ns__concat soap_tmp_ns__concat;
	struct ns__concatResponse soap_tmp_ns__concatResponse;
	char * soap_tmp_ns__stringType;
	soap_default_ns__concatResponse(soap, &soap_tmp_ns__concatResponse);
	soap_tmp_ns__stringType = NULL;
	soap_tmp_ns__concatResponse.concat = &soap_tmp_ns__stringType;
	soap_default_ns__concat(soap, &soap_tmp_ns__concat);
	if (!soap_get_ns__concat(soap, &soap_tmp_ns__concat, "ns:concat", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = ns__concat(soap, soap_tmp_ns__concat.concatRequest, soap_tmp_ns__concatResponse.concat);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL; /* use SOAP literal style */
	soap_serializeheader(soap);
	soap_serialize_ns__concatResponse(soap, &soap_tmp_ns__concatResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__concatResponse(soap, &soap_tmp_ns__concatResponse, "ns:concatResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__concatResponse(soap, &soap_tmp_ns__concatResponse, "ns:concatResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_ns__adder(struct soap *soap)
{	struct ns__adder soap_tmp_ns__adder;
	struct ns__adderResponse soap_tmp_ns__adderResponse;
	long soap_tmp_ns__longType;
	soap_default_ns__adderResponse(soap, &soap_tmp_ns__adderResponse);
	soap_default_ns__longType(soap, &soap_tmp_ns__longType);
	soap_tmp_ns__adderResponse.adder = &soap_tmp_ns__longType;
	soap_default_ns__adder(soap, &soap_tmp_ns__adder);
	if (!soap_get_ns__adder(soap, &soap_tmp_ns__adder, "ns:adder", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = ns__adder(soap, soap_tmp_ns__adder.adderRequest, soap_tmp_ns__adderResponse.adder);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL; /* use SOAP literal style */
	soap_serializeheader(soap);
	soap_serialize_ns__adderResponse(soap, &soap_tmp_ns__adderResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__adderResponse(soap, &soap_tmp_ns__adderResponse, "ns:adderResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__adderResponse(soap, &soap_tmp_ns__adderResponse, "ns:adderResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_ns__bye(struct soap *soap)
{	struct ns__bye soap_tmp_ns__bye;
	struct ns__byeResponse bye;
	soap_default_ns__byeResponse(soap, &bye);
	soap_default_ns__bye(soap, &soap_tmp_ns__bye);
	if (!soap_get_ns__bye(soap, &soap_tmp_ns__bye, "ns:bye", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = ns__bye(soap, soap_tmp_ns__bye.byeRequest, &bye);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL; /* use SOAP literal style */
	soap_serializeheader(soap);
	soap_serialize_ns__byeResponse(soap, &bye);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__byeResponse(soap, &bye, "ns:byeResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__byeResponse(soap, &bye, "ns:byeResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

#if defined(__BORLANDC__)
#pragma option pop
#pragma option pop
#endif

/* End of soapServer.c */
