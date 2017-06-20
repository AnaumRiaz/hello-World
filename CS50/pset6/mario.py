import cs50

def main():
    
    # Getting anumber that meets our requirements
    
    print("height: ", end ="")
    number = getposint()
    print("number entered is {}".format(number))
    
    #printing out the pyramid
    for i in range(2, number + 2):
        print(" " * (number - i +1), end ="")
        print("#" * (i))
    



def getposint():
    while True:
        n = cs50.get_int()
        if n >= 0 and n < 24:
            break
        else:
            print("Retry")
    return n
    
            

if __name__ == "__main__":
        main()