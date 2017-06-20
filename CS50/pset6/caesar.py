
import cs50
import sys


def main():
    
    if len(sys.argv) != 2:
        print("You did not enter valid number ")
        exit(1)
     
    key= int(sys.argv[1])
    print("Enter string ");
    message = cs50.get_string()
    
    for i in message:
    
        if str.isalpha(i):
            
      
            if str.islower(i):
                
                inter = ord(i) + (key % 26)
                
                #ascii larger than the small z
                if (inter > 122):
                    inter = (inter % 122) + 96
   
    
            else:
                inter = ord(i) + (key % 26)
                
                #90 being the ascii of big z
                if inter > 90:
                   inter = (inter % 90) + 64
              
            print(chr(inter), end ="")
            
        
        else :
            print(i, end = "")
        
    
    print()
    exit(0)
  
  
if __name__ == "__main__":
    main()