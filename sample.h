// Reminder: Modify typemap.dat to customize the header file generated by wsdl2h
/* sample.h
   Generated by wsdl2h 2.8.122 from sample.wsdl and typemap.dat
   2022-06-07 18:28:24 GMT

   DO NOT INCLUDE THIS ANNOTATED FILE DIRECTLY IN YOUR PROJECT SOURCE CODE.
   USE THE FILES GENERATED BY soapcpp2 FOR YOUR PROJECT'S SOURCE CODE.

gSOAP XML Web services tools
Copyright (C) 2000-2022, Robert van Engelen, Genivia Inc. All Rights Reserved.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
--------------------------------------------------------------------------------
A commercial use license is available from Genivia Inc., contact@genivia.com
--------------------------------------------------------------------------------
*/

/**

@page page_notes Notes

@note HINTS:
 - Run soapcpp2 on sample.h to generate the SOAP/XML processing logic:
   Use soapcpp2 -I to specify paths for #import
   Use soapcpp2 -j to generate improved proxy and server classes.
   Use soapcpp2 -r to generate a report.
 - Edit 'typemap.dat' to control namespace bindings and type mappings:
   It is strongly recommended to customize the names of the namespace prefixes
   generated by wsdl2h. To do so, modify the prefix bindings in the Namespaces
   section below and add the modified lines to 'typemap.dat' to rerun wsdl2h.
 - Run Doxygen (www.doxygen.org) on this file to generate documentation.
 - Use wsdl2h -c to generate pure C code.
 - Use wsdl2h -R to include the REST operations defined by the WSDLs.
 - Use wsdl2h -O3 or -O4 to optimize by removing unused schema components.
 - Use wsdl2h -d to enable DOM support for xsd:any and xsd:anyType.
 - Use wsdl2h -F to simulate struct-type derivation in C (also works in C++).
 - Use wsdl2h -f to generate flat C++ class hierarchy, removes type derivation.
 - Use wsdl2h -g to generate top-level root elements with readers and writers.
 - Use wsdl2h -U to map XML names to C++ Unicode identifiers instead of _xNNNN.
 - Use wsdl2h -u to disable the generation of unions.
 - Use wsdl2h -L to remove this @note and all other @note comments.
 - Use wsdl2h -nname to use name as the base namespace prefix instead of 'ns'.
 - Use wsdl2h -Nname for service prefix and produce multiple service bindings
 - Struct/class members serialized as XML attributes are annotated with a '@'.
 - Struct/class members that have a special role are annotated with a '$'.

@warning
   DO NOT INCLUDE THIS ANNOTATED FILE DIRECTLY IN YOUR PROJECT SOURCE CODE.
   USE THE FILES GENERATED BY soapcpp2 FOR YOUR PROJECT'S SOURCE CODE:
   THE GENERATED soapStub.h FILE CONTAINS THIS CONTENT WITHOUT ANNOTATIONS.

@copyright LICENSE:
@verbatim
--------------------------------------------------------------------------------
gSOAP XML Web services tools
Copyright (C) 2000-2021, Robert van Engelen, Genivia Inc. All Rights Reserved.
The wsdl2h tool and its generated software are released under the GPL.
This software is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
--------------------------------------------------------------------------------
GPL license.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place, Suite 330, Boston, MA 02111-1307 USA

Author contact information:
engelen@genivia.com / engelen@acm.org

This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
--------------------------------------------------------------------------------
A commercial-use license is available from Genivia, Inc., contact@genivia.com
--------------------------------------------------------------------------------
@endverbatim

*/


//gsoapopt c,w

/******************************************************************************\
 *                                                                            *
 * Definitions                                                                *
 *   http://mediStor.org/x-rays                                               *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * $SIZE typemap variable:                                                    *
 *   int                                                                      *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Import                                                                     *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Schema Namespaces                                                          *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Built-in Schema Types and Top-Level Elements and Attributes                *
 *                                                                            *
\******************************************************************************/

/// Built-in type "xs:base64Binary".
struct xsd__base64Binary
{
    unsigned char *__ptr;
    int __size;
    char *id, *type, *options; // NOTE: non-NULL for DIME/MIME/MTOM XOP attachments only
};

/// Built-in type "xs:boolean".
enum xsd__boolean { xsd__boolean__false_, xsd__boolean__true_ };


/******************************************************************************\
 *                                                                            *
 * Services                                                                   *
 *                                                                            *
\******************************************************************************/


//gsoap ns1  service name:	xRayStorageBinding 
//gsoap ns1  service type:	xRayStorage 
//gsoap ns1  service port:	http://localhost:9000 
//gsoap ns1  service namespace:	http://mediStor.org/x-rays 
//gsoap ns1  service transport:	http://schemas.xmlsoap.org/soap/http 

/** @mainpage XrayStorage Definitions

@section XrayStorage_bindings Service Bindings

  - @ref xRayStorageBinding

@section XrayStorage_more More Information

  - @ref page_notes "Notes"

  - @ref page_XMLDataBinding "XML Data Binding"

  - @ref SOAP_ENV__Header "SOAP Header Content" (when applicable)

  - @ref SOAP_ENV__Detail "SOAP Fault Detail Content" (when applicable)


*/

/** @page xRayStorageBinding Binding "xRayStorageBinding"

@section xRayStorageBinding_operations Operations of Binding "xRayStorageBinding"

  - @ref __ns1__store

@section xRayStorageBinding_ports Default endpoints of Binding "xRayStorageBinding"

  - http://localhost:9000

@note Use wsdl2h option -Nname to change the service binding prefix name


*/

/******************************************************************************\
 *                                                                            *
 * Service Binding                                                            *
 *   xRayStorageBinding                                                       *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Service Operation                                                          *
 *   __ns1__store                                                             *
 *                                                                            *
\******************************************************************************/


/** Operation "__ns1__store" of service binding "xRayStorageBinding".

  - SOAP document/literal style messaging

  - Default endpoints:
    - http://localhost:9000

  - Addressing input action: ""

  - Addressing output action: "Response"

  - Request message MIME multipart/related attachments:
    -# MIME attachment with SOAP Body and mandatory header part(s):
    -# MIME attachment 2:
       - part: xRay
       - type: image/jpeg

C stub function (defined in soapClient.c[pp] generated by soapcpp2):
@code
  int soap_call___ns1__store(
    struct soap *soap,
    NULL, // char *endpoint = NULL selects default endpoint for this operation
    NULL, // char *action = NULL selects default action for this operation
    // input parameters:
    char*                               patientName,
    int                                 patientNumber,
    struct xsd__base64Binary            xRay,
    // output parameters:
    enum xsd__boolean                  *success
  );
@endcode

C server function (called from the service dispatcher defined in soapServer.c[pp]):
@code
  int __ns1__store(
    struct soap *soap,
    // input parameters:
    char*                               patientName,
    int                                 patientNumber,
    struct xsd__base64Binary            xRay,
    // output parameters:
    enum xsd__boolean                  *success
  );
@endcode

*/

//gsoap ns1  service method-protocol:	store SOAP
//gsoap ns1  service method-style:	store document
//gsoap ns1  service method-encoding:	store literal
//gsoap ns1  service method-action:	store ""
//gsoap ns1  service method-output-action:	store Response
//gsoap ns1  service method-input-mime-type:	store image/jpeg
int __ns1__store(
    char*                               patientName,	///< Input parameter
    int                                 patientNumber,	///< Input parameter
    struct xsd__base64Binary            xRay,	///< Input parameter
    enum xsd__boolean                  *success	///< Output parameter
);

/** @page xRayStorageBinding Binding "xRayStorageBinding"

@section xRayStorageBinding_policy_enablers Policy Enablers of Binding "xRayStorageBinding"

None specified.

*/

/* End of sample.h */
