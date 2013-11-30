def OR(ip1,ip2,op):
    print "Nand(a=%s,b=%s,out=not%s);" % (ip1,ip1,ip1)
    print "Nand(a=%s,b=%s,out=not%s);" % (ip2,ip2,ip2)
    print "Nand(a=not%s,b=not%s,out=%s);\n" %(ip1,ip2,op)

def NOT(ip,op):
    print "Nand(a=%s, b=%s, out=%s); \n" % (ip,ip,op)

def AND(ip1,ip2,op,ctr):
    print "Nand(a=%s, b=%s, op=temp%d);" % (ip1,ip2,ctr)
    print "Nand(a=temp%d, b= temp%d, out=%s);" % (ctr,ctr,op)

def XOR(ip1,ip2,op,ctr):
    print "// Not %s" % ip1
    print "Nand(a=%s,b=%s,out=not%s);\n" % (ip1,ip1,ip1)
    print "// Not %s" % ip2
    print "Nand(a=%s,b=%s,out=not%s);\n" % (ip2,ip2,ip2)
    print "// And %s and not%s (partially)" % (ip1,ip2)
    print "Nand(a=%s,b=not%s,out=temp%d);\n" % (ip1,ip2,ctr)
    print "// And %s and not%s (partially)" % (ip2,ip1)
    print "Nand(a=%s,b=not%s,out=temp%d); \n" % (ip2,ip1,ctr+1)
    print "Nand(a=ctr%d,b=ctr%d,out=%s);" %(ctr,ctr+1,op)


def main():
    ctr = 0
    inp = raw_input("Chip name: \n").upper()
    while True:
        if inp == "OR":
            arr = raw_input("Enter ip1, ip2 and op in order. (ip1 ip2 op) \n").split(" ")
            OR(arr[0],arr[1],arr[2])
        elif inp == "AND":
            arr = raw_input("Enter ip1, ip2 and op in order. (ip1 ip2 op) \n").split(" ")
            AND(arr[0],arr[1],arr[2],ctr)
            ctr += 1
        elif inp == "NOT":
            arr = raw_input("Enter ip and op in order. (ip op) \n").split(" ")
            NOT(arr[0],arr[1])
        elif inp == "XOR":
            arr = raw_input("Enter ip1, ip2 and op in order. (ip1 ip2 op) \n").split(" ")
            XOR(arr[0],arr[1],arr[2],ctr)
            ctr += 1
        elif inp == "END": break 
        else:
            print "Invalid name, please try again. \n"
        inp = raw_input("Chip name: \n").upper()

main()
        
            
            
            
    
    
