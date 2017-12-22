


all: parser.out RSA.out

parser.out: test_parser.c msglib.c
	gcc test_parser.c msglib.c -o parser.out

RSA.out: test_RSA.c RSA.c longdiv.c msglib.c
	gcc test_RSA.c RSA.c longdiv.c msglib.c -o RSA.out

