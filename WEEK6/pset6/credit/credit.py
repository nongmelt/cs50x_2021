from cs50 import get_string
number = get_string("Number: ")[::-1]
even, odd = 0, 0
for i in range(len(number)) :
    if i%2 == 1 :
        if int(number[i])*2 > 9 :
            even += 1 + (int(number[i])*2)%10
        else :
            even += int(number[i])*2
    else :
        odd += int(number[i])
check = even + odd
if len(number) < 14 or len(number) > 17 or check%10 != 0 :
    print("INVALID")
else :
    number = number[::-1]
    if number[0:2] in ['34','37'] :
        print("AMEX")
    elif number[0:2] in ['51','52','53','54','55'] :
        print("MASTERCARD")
    elif number[0] == '4' :
        print("VISA")
    else :
        print("INVALID")