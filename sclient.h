//gsoap ns service name: SampleServices 

typedef struct concatStruct {
  long id 1;
  char * op1 1;
  char * op2 1;
} ns__concatType;

typedef struct addStruct {
  long id 1;
  long op1 1;
  long op2 1;
} ns__addType ;

typedef struct byeStruct {
  long id 1 ;
} ns__byeType ;

struct xsd__base64Binary {
    unsigned char *__ptr; // pointer to binary data
    int __size;           // size of the binary data
    char *id;             // NULL to generate an id or assign this member variable a unique UUID
    char *type;           // MIME type of the data
    char *options;        // DIME options or an optional description of the MIME attachment
};


int ns__webmethod(struct xsd__base64Binary *data, struct xsd__base64Binary *result);

typedef char * ns__stringType  ;
typedef long ns__longType  ;

int ns__connect (ns__longType *connect) ; // NO REQUEST!

int ns__echo (ns__stringType echoRequest 1, ns__stringType *echo) ;

int ns__concat (ns__concatType concatRequest 1, ns__stringType *concat) ;

int ns__adder (ns__addType adderRequest 1, ns__longType *adder) ;

int ns__bye (ns__byeType byeRequest 1, struct ns__byeResponse { } *bye); // NO RESPONSE 

