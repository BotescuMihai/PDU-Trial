#include <pthread.h>

#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include "SampleServices.nsmap"
#include "soapH.h"

/*
extern WINDOW * mainwnd ;
extern pthread_mutex_t curmtx ;

WINDOW *soapwnd; 
*/

/* Reuse this function. Previously from inetds.c */

long create_client_id () ;
void closewin (WINDOW *wnd) ;

void *soap_main (void *args) {
  struct soap soap ;
  int msd, csd, port, reuseAddrON = 1 ;
  port = *(int *)(args);
  soap_init (&soap) ;
  
  soap.bind_flags=SO_REUSEADDR ;
  msd = soap_bind (&soap, "127.0.0.1", port, 100) ;
  if (!soap_valid_socket(msd)) {
    soap_print_fault (&soap, stderr) ; 
    pthread_exit (NULL) ;
  } else {
    setsockopt(msd, SOL_SOCKET, SO_REUSEADDR, &reuseAddrON, sizeof(reuseAddrON)) ; 
  }

  for (;;) {
    csd = soap_accept (&soap) ;
    if (csd < 0) { soap_print_fault (&soap, stderr); break ; }
//    soapwnd = newwin (16, 70, 5, 5) ;
    if (soap_serve (&soap) != SOAP_OK) soap_print_fault (&soap, stderr) ;
     soap_destroy (&soap) ;
    soap_end (&soap) ;
  }
  soap_done (&soap) ;
  pthread_exit (NULL) ;
}

char *do_client_concat (struct soap *soap, char *o1, char *o2) {
  int bsize ;
  char *b ; 
  bsize = strlen (o1) + strlen (o2)+2 ;
  b = soap_malloc (soap, bsize) ;
  sprintf (b, "%s %s", o2, o1) ; b [bsize] = 0 ;



  return b ;

}
 
int ns__bye (struct soap*s, struct byeStruct rq, struct ns__byeResponse *rsp) {
  /* Do nothing for this sample. 
     In "Real" application clear structures associated with your ID */
  return SOAP_OK ;
}

int ns__connect(struct soap*s, long *rsp) {
  long clientid = create_client_id() ;
  *rsp = clientid ;

  return SOAP_OK ;
}
int my_get(struct soap *soap, char * rq)
{
    const char *ptr = rq;
    size_t size = strlen(ptr);
    soap_set_mime(soap, NULL, "body");
    soap_set_mime_attachment(soap, ptr, size, SOAP_MIME_NONE, "text/xml", "attach1", NULL, NULL);
    soap->http_content = "text/xml";
    if (soap_response(soap, SOAP_FILE)
        || soap_send(soap, "<doc title=\"Example\">Some text</doc>\n")
        || soap_end_send(soap))
        return soap_closesock(soap);
    soap_clr_mime(soap);
    return SOAP_OK;
}
int ns__echo(struct soap*s, char *rq, char **rsp) {
    char *rsp1 ;
    int n = 0;
    /*
    struct soap_multipart *attachment;
    printf("Attachements\n");
    for (attachment = s->dime.list; attachment; attachment = attachment->next)
    {
        ++n;
        printf("Part %d:\n", n);
        printf("ptr        =%p\n", attachment->ptr);
        printf("size       =%ul\n", attachment->size);
        printf("id         =%s\n", attachment->id ? attachment->id : "");
        printf("type       =%s\n", attachment->type ? attachment->type : "");
        printf("location   =%s\n", attachment->location ? attachment->location : "");
        printf("description=%s\n", attachment->description ? attachment->description : "");
    }

    rsp1 = do_client_concat (s, ":: Am primit ::", rq) ;
    *rsp = rsp1 ;
    fprintf(stderr, "File content\n===========\n");
    fprintf(stderr, "%s\n", *rsp); */
    return SOAP_OK ;
}

int ns__webmethod(struct soap *soap, struct xsd__base64Binary *data, struct xsd__base64Binary *result)
{
    // echo back the structure (as a MIME attachment)
    result->__ptr = data->__ptr;
    result->__size = data->__size;
    result->id = data->id;
    result->type = data->type;
    result->options = data->options;
    struct soap_multipart *attachment;
    printf("Attachements\n");
    for (attachment = soap->dime.list; attachment; attachment = attachment->next)
    {
        ++n;
        printf("Part %d:\n", n);
        printf("ptr        =%p\n", attachment->ptr);
        printf("size       =%ul\n", attachment->size);
        printf("id         =%s\n", attachment->id ? attachment->id : "");
        printf("type       =%s\n", attachment->type ? attachment->type : "");
        printf("location   =%s\n", attachment->location ? attachment->location : "");
        printf("description=%s\n", attachment->description ? attachment->description : "");
    }
    return SOAP_OK;
}

/*
 <?xml version="1.0" encoding="UTF-8"?>
<SOAP-ENV:Envelope
    xmlns:SOAP-ENV="http://schemas.xmlsoap.org/soap/envelope/"
    xmlns:SOAP-ENC="http://schemas.xmlsoap.org/soap/encoding/"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xmlns:xsd="http://www.w3.org/2001/XMLSchema"
    xmlns:ns="http://tempuri.org/ns.xsd">
 <SOAP-ENV:Body>
 <mtom:AttachmentRequest>
 <mtom:fileName>file</mtom:fileName>
  <ns:echo>
   <echoRequest>Hei</echoRequest>
  </ns:echo>
 </SOAP-ENV:Body>
</SOAP-ENV:Envelope>

 ALA BUN
 */

/**
 * <?xml version="1.0" ?>
 * <wsdl:definitions name="DIME Sample" targetNamespace="https://example.com/DimeExample" xmlns:wsdl="https://schemas.xmlsoap.org/wsdl/" xmlns:soap="https://schemas.xmlsoap.org/wsdl/soap/" xmlns:dime="https://schemas.xmlsoap.org/ws/2002/04/dime/wsdl/" xmlns:content= "https://schemas.xmlsoap.org/ws/2002/04/content-type/" xmlns:ref="https://schemas.xmlsoap.org/ws/2002/04/reference/" xmlns:svc="https://example.com/DimeExample/Service/" xmlns:msg="https://example.com/DimeExample/Messages/" xmlns:xs="https://www.w3.org/2001/XMLSchema" >
 * <wsdl:types> <xs:schema targetNamespace="https://example.com/DimeExample" elementFormDefault="qualified"> <xs:import namespace= "https://schemas.xmlsoap.org/ws/2002/04/content-type/"/> <xs:import namespace= "https://schemas.xmlsoap.org/ws/2002/04/reference/"/> <xs:element name="GetMediaFile"> <xs:complexType> <xs:sequence> <xs:element name="fileName" type="xs:string"/> <xs:element name="file" type="msg:MediaFile" minOccurs="0"/> </xs:sequence> </xs:complexType> </xs:element> <xs:complexType name="ReferencedBinary"> <xs:simpleContent>
 * <xs:extension base="xs:base64Binary"> <xs:attribute ref="ref:location" use="optional"/> </xs:extension> </xs:simpleContent> </xs:complexType> <xs:complexType name="MediaFile"> <xs:simpleContent> <xs:restriction base="msg:ReferencedBinary"> <xs:annotation> <xs:appinfo> <content:mediaType type="video/mpeg"/> </xs:appinfo> </xs:annotation> </xs:restriction> </xs:simpleContent> </xs:complexType> </xs:schema> </wsdl:types> <wsdl:message name="GetMediaFile"> <wsdl:part name="body" element="msg:GetMediaFile"/> </wsdl:message> <wsdl:portType name="PortType">
 * <wsdl:operation name="GetMediaFile"> <wsdl:input message="svc:GetMediaFile"/> </wsdl:operation> </wsdl:portType> <wsdl:binding name="SoapDimeBinding" type="svc:PortType"> <soap:binding transport="https://schemas.xmlsoap.org/soap/http" style="document"/> <wsdl:operation name="GetMediaFile"> <soap:operation soapAction="https://example.com/GetMediaFile"/> <wsdl:input> <dime:message layout= "https://schemas.xmlsoap.org/ws/2002/04/dime/closed-layout" wsdl:required="true"/> <soap:body parts="body" use="literal"/> </wsdl:input> </wsdl:operation> </wsdl:binding>
 * <wsdl:service name="DimeSample"> <wsdl:port name="SoapDimePort" binding="svc:SoapDimeBinding"> <soap:address location="https://example.com/DimeSample/"/> </wsdl:port> </wsdl:service> </wsdl:definitions>
 * @param s
 * @param rq
 * @param rsp
 * @return
 */
/*
int ns__echo(struct soap*s, char *rq, char **rsp) {
  char *rsp1 ;
  rsp1 = do_client_concat (s, ":: echo ::", rq) ;
  *rsp = rsp1 ;
  return SOAP_OK ;
}*/

int ns__concat(struct soap*s, struct concatStruct rq, char **rsp) {
  char *rsp1;
  rsp1 = do_client_concat (s, rq.op1, rq.op2) ;
  *rsp = rsp1 ; 
  return SOAP_OK ;
}

int ns__adder(struct soap*s, struct addStruct rq, long *rsp) {
  long rsp1 ;
  rsp1 = rq.op1 + rq.op2; 
  *rsp = rsp1 ;

  return SOAP_OK ;
}



