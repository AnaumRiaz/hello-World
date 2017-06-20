
import cs50


def main():
    
    coins = 0
    calc_coins = 0

    # Asking for chage from user till he enters a non negative number*/
     
 
    while True:
    
        print("O hai! How much change is owed?");
        inp_change = cs50.get_float()
        if inp_change >= 0:
            break
    
  
    change = round(inp_change * 100)  
 
    # Checks whether the change will use 25C or 10C coins etc  by dividing with the appropriate number
    # saves the number of coins in coins
       
   
    if change >= 25:
        calc_coin = int(change / 25)
        change = change - calc_coin * 25
        coins += calc_coin
        
             
    if change >= 10: 
        calc_coin = int(change / 10)
        change = change - calc_coin * 10
        coins += calc_coin        
             
    if change >= 5:
        calc_coin = int(change / 5)
        change = change - calc_coin * 5
        coins += calc_coin 
    
              
    if change < 5:
        calc_coin = change / 1
        change = change - calc_coin* 1
        coins += calc_coin        
          
    print(int(coins))  


if __name__ == "__main__":
    main()
