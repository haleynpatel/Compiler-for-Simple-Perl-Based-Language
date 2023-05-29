#include "val.h"
// update in future: change all ints to real before doing ops to save time
//if one or both operands are not numeric, an attempt is made to conver the non-numeric operand to a numeric one
Value Value::operator+(const Value& op) const{
    double conv;
    double oper;
    if(IsReal() && op.IsReal()){
        return Value(Rtemp+op.Rtemp);
    }
    else if(IsInt() && op.IsInt()){
        return Value(GetReal()+op.GetReal());
    }
    else if((IsReal()&&op.IsInt())||(IsInt() && op.IsReal())){
        return Value(GetReal()+op.GetReal());
    }
    else if(IsReal()&& op.IsString()){
        try{
            conv = stod(op.GetString());
        }
        catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double."<<endl;
            return Value();
        }
        return Value(Rtemp+conv);
    }
    else if(IsString()&& op.IsReal()){
        try{
            conv = stod(GetString());
        }
        catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double."<<endl;
            return Value();
        }
        return Value(conv+op.Rtemp);        
    }
    else if(IsString()&&op.IsString()){
        try{
            conv = stod(GetString());
        }
        catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double."<<endl;;
            return Value();
        }
        try{
            oper = stod(op.GetString());
        }
        catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double."<<endl;
            return Value();
        }
        return Value(conv+oper);  
    }
    else if(IsBool() || op.IsBool()){
        return Value();
    }
    else{
        return Value();
    }
}
Value Value::operator-(const Value& op)const{
    double conv;
    double oper;
    
    if(IsReal() && op.IsReal()){
        return Value(GetReal()-op.GetReal());
    }
    else if(IsInt() && op.IsInt()){
        
        return Value(GetInt()-op.GetInt());
    }
    else if((IsReal()&&op.IsInt())||(IsInt() && op.IsReal())){
        return Value(GetReal()-op.GetReal());
    }
    else if(IsReal()&& op.IsString()){
        try{
            conv = stod(op.GetString());
        }
        catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double."<<endl;
            return Value();
        }
        return Value(Rtemp-conv);
    }
    else if(IsString()&& op.IsReal()){
        try{
            conv = stod(GetString());
        }
        catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double."<<endl;
            return Value();
        }
        return Value(conv-op.Rtemp);        
    }
    else if(IsString()&&op.IsString()){
        try{
            conv = stod(GetString());
        }
        catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double."<<endl;;
            return Value();
        }
        try{
            oper = stod(op.GetString());
        }
        catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double."<<endl;
            return Value();
        }
        return Value(conv-oper);  
    }
    else{
        return Value();
    }
}

Value Value::operator*(const Value& op) const{
    double conv;
    double oper;
    if(IsReal() && op.IsReal()){
        return Value(Rtemp*op.Rtemp);
    }
    else if(IsInt() && op.IsInt()){
        return Value(GetReal()*op.GetReal());
    }
    else if((IsReal()&&op.IsInt())||(IsInt() && op.IsReal())){
        return Value(GetReal()*op.GetReal());
    }
    else if(IsReal()&& op.IsString()){
        try{
            conv = stod(op.GetString());
        }
        catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double."<<endl;
            return Value();
        }
        return Value(Rtemp*conv);
    }
    else if(IsString()&& op.IsReal()){
        try{
            conv = stod(GetString());
        }
        catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double."<<endl;
            return Value();
        }
        return Value(conv*op.Rtemp);        
    }
    else if(IsString()&&op.IsString()){
        try{
            conv = stod(GetString());
        }
        catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double."<<endl;;
            return Value();
        }
        try{
            oper = stod(op.GetString());
        }
        catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double."<<endl;
            return Value();
        }
        return Value(conv*oper);  
    }
    else{
        return Value();
    }
}

Value Value::operator/(const Value& op) const{
    double conv;
    double oper;
    if(IsReal() && op.IsReal()){
        if(op.GetReal() ==0 ){
            cout << "Divide by 0 Error" << endl;
            return Value();
        }
        return Value(Rtemp/op.Rtemp);
    }
    else if(IsInt() && op.IsInt()){
        if(op.GetInt() == 0){
            cout << "Divide by 0 Error" << endl;
            return Value();            
        }
        return Value(GetReal()/op.GetReal());
    }
    else if((IsReal()&&op.IsInt())||(IsInt() && op.IsReal())){
        if(op.GetReal() == 0){
            cout << "Divide by 0 Error" << endl;
            return Value();               
        }
        return Value(GetReal()/op.GetReal());
    }
    else if(IsReal()&& op.IsString()){
        try{
            conv = stod(op.GetString());
        }
        catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double."<<endl;
            return Value();
        }
        if(conv == 0){
            cout << "Divide by 0 Error" << endl;
            return Value();  
        }
        return Value(Rtemp/conv);
    }
    else if(IsString()&& op.IsReal()){
        try{
            conv = stod(GetString());
        }
        catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double."<<endl;
            return Value();
        }
        if(op.Rtemp == 0){
            cout << "Divide by 0 Error" << endl;
            return Value();  
        }
        return Value(conv/op.Rtemp);        
    }
    else if(IsString()&&op.IsString()){
        try{
            conv = stod(GetString());
        }
        catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double."<<endl;;
            return Value();
        }
        try{
            oper = stod(op.GetString());
        }
        catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double."<<endl;
            return Value();
        }
        if(oper == 0){
            cout << "Divide by 0 Error" << endl;
            return Value();  
        }
        return Value(conv/oper);  
    }
    else{
        return Value();
    }
}

//Operate on numeric only, return true or false
Value Value::operator==(const Value& op) const{
    double conv;
    double oper;
    if (IsReal() && op.IsReal()) {
        return Value(Rtemp == op.Rtemp);
    }
    else if (IsInt() && op.IsInt()) {
        return Value(Itemp == op.Itemp);
    }
    else if((IsInt() && op.IsReal()) ||(IsReal() && op.IsInt())){
        return Value(GetReal() == op.GetReal());
    }
    else if(IsReal()&& op.IsString()){
        try{
            conv = stod(op.GetString());
        }
        catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double."<<endl;
            return Value();
        }
        return Value(GetReal()==conv);
    }
    else if(IsString()&& op.IsReal()){
        try{
            conv = stod(GetString());
        }
        catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double."<<endl;
            return Value();
        }
        return Value(conv==op.Rtemp);        
    }    
    else if(IsString()&&op.IsString()){
        try{
            conv = stod(GetString());
        }
        catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double."<<endl;
            return Value();
        }
        try{
            oper = stod(op.GetString());
        }
        catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double."<<endl;;
            return Value();
        }
        return Value(conv==oper);  
    }
    else {
        return Value();
    }    
}

Value Value::operator>(const Value& op) const{
    double conv;
    double oper;
    if (IsReal() && op.IsReal()) {
        return Value(Rtemp > op.Rtemp);
    }
    else if (IsInt() && op.IsInt()) {
        return Value(Itemp > op.Itemp);
    }
    else if((IsInt() && op.IsReal()) ||(IsReal() && op.IsInt())){
        return Value(GetReal() > op.GetReal());
    }
    else if(IsReal()&& op.IsString()){
        try{
            conv = stod(op.GetString());
        }
        catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double."<<endl;
            return Value();
        }
        return Value(GetReal()>conv);
    }
    else if(IsString()&& op.IsReal()){
        try{
            conv = stod(GetString());
        }
        catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double."<<endl;
            return Value();
        }
        return Value(conv>op.Rtemp);        
    }    
    else if(IsString()&&op.IsString()){
        try{
            conv = stod(GetString());
        }
        catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double."<<endl;
            return Value();
        }
        try{
            oper = stod(op.GetString());
        }
        catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double."<<endl;;
            return Value();
        }
        return Value(conv>oper);  
    }
    else {
        return Value();
    }    
}

Value Value::operator<(const Value& op) const{
    double conv;
    double oper;
    if (IsReal() && op.IsReal()) {
        return Value(Rtemp < op.Rtemp);
    }
    else if (IsInt() && op.IsInt()) {
        return Value(Itemp < op.Itemp);
    }
    else if((IsInt() && op.IsReal()) ||(IsReal() && op.IsInt())){
        return Value(GetReal() < op.GetReal());
    }
    else if(IsReal()&& op.IsString()){
        try{
            conv = stod(op.GetString());
        }
        catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double."<<endl;
            return Value();
        }
        return Value(GetReal()<conv);
    }
    else if(IsString()&& op.IsReal()){
        try{
            conv = stod(GetString());
        }
        catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double."<<endl;
            return Value();
        }
        return Value(conv<op.Rtemp);        
    }    
    else if(IsString()&&op.IsString()){
        try{
            conv = stod(GetString());
        }
        catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double."<<endl;
            return Value();
        }
        try{
            oper = stod(op.GetString());
        }
        catch(invalid_argument & arg){
            cout << "Invalid conversion from string to double."<<endl;;
            return Value();
        }
        return Value(conv<oper);  
    }
    else {
        return Value();
    }  

}
Value Value::operator^(const Value& op) const{
    if(IsReal() && op.IsReal()){
        return Value(pow(Rtemp, op.Rtemp));
    }
    else if(IsInt()&&op.IsInt()){
        return Value(pow(Itemp,op.Itemp));
    }
    else if((IsReal()&& op.IsInt()) || (IsInt() && op.IsReal())){
        return Value(pow(GetReal(), op.GetReal()));
    }
    else{
        return Value();
    }
}
//performed on two string operands. Attempt to convert string otherwise semantic error
Value Value::Catenate(const Value& oper) const {
    string conv;
    string op;
    ostringstream s;
    ostringstream t;

    if(IsString() && oper.IsString()){
        return Value(GetString()+oper.GetString());
    }
    else if(IsString() && oper.IsReal()){
        s << oper.GetReal();
        try{
            op = s.str();
        }
        catch(invalid_argument & arg){
            return Value();
        }
    return Value(GetString() + op);
    }
    else if(IsReal() && oper.IsString()){
        s << GetReal();
        try{
            conv = s.str();
        }
        catch(invalid_argument & arg){
            return Value();
        }
    return Value(conv + oper.GetString());
    }
    else if(IsReal() && oper.IsReal()){
        s << GetReal();
        try{
            conv = s.str();
        }
        catch(invalid_argument & arg){
            return Value();
        }
        t << oper.GetReal();
        try{
            op = t.str();
        }
        catch(invalid_argument & arg){
            return Value();
        }
        return Value(conv+op);  
    }
    else{
        return Value();
    }
}
Value Value::Repeat(const Value& op) const {
    //First operands is a string and the second operand is an integer
    double oper;
    if( IsString() && op.IsString()){
    //second operand must be converted to a numeric
        try {
            oper = stod(op.GetString());
        }
        catch( invalid_argument & arg){
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }

        int times = oper;
        string str = GetString(), newstr = "";
        for(int i = 1; i <= times; i++){
            newstr += str;
        }
        return Value( newstr );
    }
    else if(IsReal() && op.IsString()){
    //Both operands to be converted
    //first operand must be converted to a string
    ostringstream outStr1;
    outStr1 << GetReal();
    string stroper1 = outStr1.str(), newstr = "";
    //second operand must be converted to a numeric
        try {
            oper = stod(op.GetString());
        }
        catch( invalid_argument & arg){
            cout << "Invalid conversion from string to double." << endl;
            return Value();
        }
        int times = oper;
        for(int i = 1; i <= times; i++){
           newstr += stroper1; 
        }
        return Value( newstr );
        }
    else if(IsString() && op.IsReal()){
        string newstr = "";
        int times = op.GetReal();
        for(int i = 1; i<= times; i++){
            newstr += GetString();
        }
        return Value(newstr);
    }
    else if(IsString() && op.IsInt()){
        string newstr = "";
        int times = op.GetInt();
        for(int i = 1; i<= times; i++){
            newstr += GetString();
        }
        return Value(newstr);
    }
    else if(IsReal() && op.IsReal()){
        ostringstream outStr1;
        outStr1 << GetReal();
        string stroper1 = outStr1.str(), newstr = "";
        int times = op.GetReal();
        for(int i = 1; i <= times; i++){
           newstr += stroper1; 
        }
        return Value( newstr );
    }
    else{
        return Value();
    }
}
//operate on string operands, produce true or false

Value Value::SEqual(const Value& oper) const {
    ostringstream s;
    ostringstream t;
    string conv;
    string op;
    if(IsString()&&oper.IsString()){
        return Value(GetString()==oper.GetString());
    }
        else if(IsString() && oper.IsReal()){
        s << oper.GetReal();
        try{
            op = s.str();
        }
        catch(invalid_argument & arg){
            return Value();
        }
    return Value(GetString() == op);
    }
    else if(IsReal() && oper.IsString()){
        s << GetReal();
        try{
            conv = s.str();        }
        catch(invalid_argument & arg){
            return Value();
        }
    return Value(conv == oper.GetString());
    }
    else if(IsReal() && oper.IsReal()){
        s << GetReal();
        try{
            conv = s.str();        }
        catch(invalid_argument & arg){
            return Value();
        }
        t << oper.GetReal();
        try{
            op = t.str();
        }
        catch(invalid_argument & arg){
            return Value();
        }
        return Value(conv<op);  
    }
    else{
        return Value();
    }
}
Value Value::SLthan(const Value& oper) const {
    ostringstream s;
    ostringstream t;
    string conv;
    string op;
    if(IsString()&&oper.IsString()){
        return Value(GetString()<oper.GetString());
    }
        else if(IsString() && oper.IsReal()){
        s << oper.GetReal();
        try{
            op = s.str();
        }
        catch(invalid_argument & arg){
            return Value();
        }
    return Value(GetString() < op);
    }
    else if(IsReal() && oper.IsString()){
        s << GetReal();
        try{
            conv = s.str();
        }
        catch(invalid_argument & arg){
            return Value();
        }
    return Value(conv < oper.GetString());
    }
    else if(IsReal() && oper.IsReal()){
        s << GetReal();
        try{
            conv = s.str();
        }
        catch(invalid_argument & arg){
            return Value();
        }
        t << oper.GetReal();
        try{
            op = t.str();
        }
        catch(invalid_argument & arg){
            return Value();
        }
        return Value(conv<op);  
    }
    else{
        return Value();
    }
}
Value Value::SGthan(const Value& oper) const {
    ostringstream s;
    ostringstream t;
    string conv;
    string op;
    if(IsString()&&oper.IsString()){
        return Value(GetString()>oper.GetString());
    }
        else if(IsString() && oper.IsReal()){
        s << oper.GetReal();
        try{
            op = s.str();
        }
        catch(invalid_argument & arg){
            return Value();
        }
    return Value(GetString() > op);
    }
    else if(IsReal() && oper.IsString()){
        s << GetReal();
        try{
            conv = s.str();
        }
        catch(invalid_argument & arg){
            return Value();
        }
        
    return Value(conv > oper.GetString());
    }
    else if(IsReal() && oper.IsReal()){
        s << GetReal();
        try{
            conv = s.str();
        }
        catch(invalid_argument & arg){
            return Value();
        }
        t << oper.GetReal();
        try{
            op = t.str();
        }
        catch(invalid_argument & arg){
            return Value();
        }
        return Value(conv>op);  
    }
    else{
        return Value();
    }
}
