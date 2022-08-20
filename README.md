# dnSolver Project
- Author: Réda ID-TALEB
- Date: 03 March 2021

# Context
DNS is a distributed hierarchical system for translating symbolic addresses into IP addresses. Information is stored on servers organized into hierarchical domains. Thus, a server can delegate part of the domain names to a subordinate server.

This project uses the UDP protocol, interrogates a DNS resolver (server which translates symbolic addresses into IP addresses), analyzes the responses of requests and finally extracts the IP address of the requested domain.

The project resumes - almost - the same display as that of the `dig` command of the linux system.

# How to execute
In a shell console, type the following commands:
```
make all
```

Then:
```
./dnsolver www.google.com
```

The format is : 
```
./dnsolver <address> 
```

To clean the project, type:
```
make clean
```

# After execution
After execution, the program displays several sections:
- **Header**: The header describes the packet type and the fields contained in the packet. Following the header are a number of additional questions, answers, authority records and records.

- **Question**: Returns the request entered by the user, indicating the type (A = Ipv4, AAAA = ipv6...) of the request, as well as its class (IN = Internet)

- **Answer**: Each line of this section forms an answer and contains:
    - The domain name queried, in the same format as the QNAME in the questions
    - The type of address (A = ipv4, AAAA = ipv6, CNAME = canonical name).
    - The class of the address (IN = Internet)
    - The response data, the format depends on the type of the address. For example, this field can contain an ipv4 address, a canonical address or a server name...

- **Datagram**: The datagram represents all the sections presented above in hexadecimal form. This datagram is decompressed, so does not contain any pointers (all pointers are replaced by their true strings).     

# Example of an execution

```
./dnsolver www.google.com

;; <<>> MY DiG Version <<>> 
;; Got answer: 
;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 8187,
;; flags: qr rd ra, QUERY: 1, ANSWER: 1 AUTHORITY: 0, ADDITIONAL: 0

;; QUESTION SECTION:
; www.google.com.		 A	IN

;; ANSWER SECTION:
www.google.com	IN	A	142.250.184.164

;; The Decoded Datagram Received from DNS Server:

 08 BB 81 80 00 01 00 01 00 00 00 00 03 77 77 77	.���.........www
 06 67 6F 6F 67 6C 65 03 63 6F 6D 00 00 01 00 01	.google.com.....
 03 77 77 77 06 67 6F 6F 67 6C 65 03 63 6F 6D 00	.www.google.com.
 00 01 00 01 00 00 00 86 00 04 8E FA B8 A4      	.......�..����
```
