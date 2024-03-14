// 10724128 吳宇哲
// 【工具】–【編譯選項】–【編譯器】勾選【編譯時加入以下命令】
// 加入【-std=c++11】 ，即可運作。

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <cmath>
#include <new>
#include <cmath>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std ;




struct Token {
    string token ;
    int type ; // 1. table1 2. table2 3. table3 4. table4 5.labal 6. immediate 7. comment
    int place ;
    int value ;
};

struct Symbol {
    string token = "";
    int place ;
    int value = 0 ;
};

class SIC {
    private:
        string fileName ;
        vector < vector<Token>> tokenTemp ;
        vector < string > table1 ;
        //vector < string > table1Up ;
        vector < string > table2 ;
        vector < string > table3 ;
        vector < string > table4 ;
        vector < Symbol> table5 ;
        vector < Symbol> table6 ;
        vector < Symbol> table7 ;
        vector < string > objectCode ;
        vector < int > locationTemp ;
        fstream newFile;
    public :
        void readFixedTable() {

            readFile( "Table1.table", 1 ) ;
            readFile( "Table2.table", 2 ) ;
            readFile( "Table3.table", 3 ) ;
            readFile( "Table4.table", 4 ) ;
        }

        bool readFile( string input, int num ) {
            fstream theFile;
            char read ;
            theFile.open( input ); // 開啟指定檔案
            if ( theFile.is_open() ) { // 如果有打開檔案
				while( !theFile.eof() ) {
                    string readTemp = "" ;
                    //string readTemp2 = "" ;
					while( theFile.get(read) && read!='\n' ) {
                        if ( read != ' ' && read !='\t')
                        readTemp = readTemp + read ;
                    }

                    //cout << readTemp << endl ;
                    if ( num == 1 )
                        table1.push_back( readTemp ) ;
                    if ( num == 2 )
                        table2.push_back( readTemp ) ;
                    if ( num == 3 )
                        table3.push_back( readTemp ) ;
                    if ( num == 4 )
                        table4.push_back( readTemp ) ;
				} // while

    			theFile.close();
    			return true ;
    		} // if
    		else {
        		cout << "The file does not exist!" << endl ;
        		return false ;
    		} // else
        } // readFile()

        bool getToken(  ) {
            fstream theFile;
            char read ;
            string input ;
            int place ;
            int pairCheck = 0;
            string pairTemp = "" ;
            cout << "Please input the file name :   " ;
            cin >> fileName ;
            input =  fileName + ".txt" ;
            theFile.open( input ); // 開啟指定檔案
            cout << "start"  << endl;
            bool isComent = false ;
            int count = 1; //check num of the line token
            if ( theFile.is_open() ) { // 如果有打開檔案
				while( !theFile.eof() ) {
                    string readTemp = "" ;
                    vector < Token > inputLine ;
                    Token inputToken ;
                    inputToken.place = 0 ;
                    inputToken.token = "" ;
                    inputToken.type = -1 ;
                    inputToken.value = 0 ;
					while( theFile.get(read) && read!='\n' ) {
                        if (( read == '\t' || read == ' ')&& readTemp != ""  )  {
                            //int type = checkType( readTemp) ;
                            inputToken.token = readTemp ;
                            inputToken.type = checkType( inputToken.token, place) ;
                            inputToken.place = place ;
                            //cout << readTemp << "     " ;
                            if ( inputToken.type == 5 && count == 1) {
                                putInTable5( inputToken) ;
                            }
                            inputLine.push_back( inputToken ) ;
                            count ++ ;
                            readTemp = "" ;
                        }
                        else {
                            bool isOperand = checkOperand( read, place) ;
                            //cout <<read <<  "     test   " <<  isOperand << endl ;
                            if ( isOperand ) {
                                int operandPlace = place;
                                if(  strcmp(readTemp.c_str(), "" ) != 0) {
                                    if ( pairCheck == 0 && read =='\'') {
                                        pairCheck++ ;
                                        pairTemp = readTemp ;
                                        inputToken.token = pairTemp ;
                                        inputToken.type = -1 ;
                                        inputLine.push_back( inputToken ) ;
                                    }
                                    else if ( pairCheck == 1 && read =='\''){
                                        if ( strcasecmp( pairTemp.c_str() , "C") == 0 ) {
                                            inputToken.token = readTemp ;
                                            inputToken.value = readTemp.size() ;
                                            inputToken.type = 7 ;
                                            putInTable7( inputToken) ;
                                        }
                                        else if ( strcasecmp( pairTemp.c_str() , "X") == 0 ) {
                                            inputToken.token = readTemp ;
                                            inputToken.value = readTemp.size() ;
                                            inputToken.type = 6 ;
                                            putInTable6( inputToken) ;
                                        }
                                        inputLine.push_back( inputToken ) ;
                                        pairCheck-- ;
                                    }
                                    else {
                                        inputToken.token = readTemp ;
                                        inputToken.type = checkType( inputToken.token, place) ;
                                        inputToken.place = place ;
                                        if ( inputToken.type == 5 && count == 1) {
                                            putInTable5( inputToken) ;
                                        }
                                        inputLine.push_back( inputToken ) ;
                                        count ++ ;
                                    }

                                } // 符號前面不為空
                                readTemp = "" ;
                                readTemp = readTemp + read ;
                                inputToken.token = readTemp ;
                                inputToken.place = operandPlace ;
                                inputToken.type = 4 ;
                                inputLine.push_back( inputToken ) ;
                                count++ ;
                                if ( strcmp(readTemp.c_str(), "." ) == 0 )
                                    break ;
                                readTemp = "" ;
                            }
                            else if ( read != '\t' && read != ' ' )
                                readTemp = readTemp + read ;

                        }

                    }  //讀檔
                    if ( strcmp(readTemp.c_str(), "." ) == 0) {
                        string commentInput = "";
                        char commentRead ;
                        bool isFirst = false ;
                        //getline(theFile,commentInput) ;
                        while( theFile.get(commentRead) && commentRead!='\n' ) {
                                if ( !isFirst ) {
                                    if ( commentRead == ' ' ||  commentRead == '\t') ;
                                   //     cout << "sdqwd" << endl ;
                                    else {
                                        commentInput = commentInput + commentRead ;
                                        isFirst = true ;
                                    }
                                }
                                else {
                                    commentInput = commentInput + commentRead ;
                                }
                        }

                        if ( isFirst ) {
                           // cout << "comment :" << commentInput << endl ;
                            inputToken.token = commentInput ;
                            inputToken.type = 7 ;
                            inputToken.place = -1 ;
                            putInTable7( inputToken) ;
                            inputLine.push_back( inputToken ) ;
                            count++ ;
                        }

                    }
                    else if ( strcasecmp(readTemp.c_str(), "") != 0) {
                        //cout << readTemp << endl ;
                        inputToken.token = readTemp ;
                        inputToken.type = checkType( inputToken.token, place) ;
                        inputToken.place = place ;
                        inputLine.push_back( inputToken ) ;
                        count ++ ;

                    } // push back last
                    tokenTemp.push_back( inputLine) ; //push back a line
                    count = 1 ;
				} // while

                cout << endl << endl ;
				for( int i = 0 ;  i < tokenTemp.size() ; i++ ) {
                    for ( int j = 0 ; j < tokenTemp.at(i).size() ; j++ )
                        cout  << "->" << tokenTemp.at(i).at(j).token << "   \"" << tokenTemp.at(i).at(j).type << ", " <<  tokenTemp.at(i).at(j).place<< "\"  ";
                    cout << endl ;
				}
    			theFile.close();
    			for( int i = 0 ; i< table5.size() ; i++) {
                    cout <<  i << "  " <<  table5.at(i).token << endl ;
                }
                for( int i = 0 ; i< table6.size() ; i++) {
                    cout <<  i << "  " << table6.at(i).token << "   " << table6.at(i).value << endl ;
                }
                for( int i = 0 ; i< table7.size() ; i++) {
                    cout << i << "  " << table7.at(i).token << "   " << table7.at(i).value << endl ;
                }
    			return true ;
    		} // if
    		else {
        		cout << "The file does not exist!" << endl ;
        		return false ;
    		} // else



        } // getToken()

        int checkType( string input, int &place ) {
            for ( int i = 1 ; i < table1.size() ; i++ ) {
                if ( strcasecmp(input.c_str(), table1.at(i).c_str()) == 0 ) {
                    place = i ;
                    return 1 ;
                }
            } // find in the table1

            for ( int i = 1 ; i < table2.size() ; i++ ) {
                if ( strcasecmp(input.c_str(), table2.at(i).c_str()) == 0 ) {
                    place = i ;
                    return 2 ;
                }
            } // find in the table2

            for ( int i = 1 ; i < table3.size() ; i++ ) {
                if ( strcasecmp(input.c_str(), table3.at(i).c_str()) == 0 ) {
                    place = i ;
                    return 3 ;
                }
            } // find in the table3

            for ( int i = 1 ; i < table4.size() ; i++ ) {
                if ( strcasecmp(input.c_str(), table4.at(i).c_str()) == 0 ) {
                    place = i ;
                    return 4 ;
                }
            } // find in the table4

            bool isNum = true ;
            for ( int i = 0 ; i < input.size() ; i ++){
                if ( (input[i] < 48) || (input[i] > 57) ) {
                    isNum = false ;
                    break ;
                }
            }

            if ( isNum) {
                return 6 ;
            }


            return 5 ;
        } // checkType

        void putInTable5( Token &input ) {
            Symbol temp ;
            int sum = 0 ;
            temp.token = input.token ;
            for ( int i = 0 ; i < input.token.size() ; i++ ) {
                sum = sum + input.token[i] ;
            }

            sum = sum % 100 ;

            while (1) {
                if ( strcasecmp(table5.at(sum).token.c_str(), "") == 0  ) {
                    temp.place = sum ; // ???
                    temp.value = input.value ;
                    table5.at(sum) = temp ;
                    input.place = sum ;
                    break ;
                }
                else {
                    sum++ ;
                    if ( sum == 100)
                        sum = 0 ;
                }
            }
        }

        void putInTable6( Token &input ) {
            Symbol temp ;
            int sum = 0 ;
            temp.token = input.token ;
            for ( int i = 0 ; i < input.token.size() ; i++ ) {
                sum = sum + input.token[i] ;
            }

            sum = sum % 100 ;

            while (1) {
                if ( strcasecmp(table6.at(sum).token.c_str(), "") == 0  ) {
                    temp.place = sum ;
                    temp.value = input.value ;
                    table6.at(sum) = temp ;
                    input.place = sum ;
                    break ;
                }
                else {
                    sum++ ;
                    if ( sum == 100)
                        sum = 0 ;
                }
            }
        } //table6

        void putInTable7( Token &input ) {
            Symbol temp ;
            int sum = 0 ;
            temp.token = input.token ;
            for ( int i = 0 ; i < input.token.size() ; i++ ) {
                sum = sum + input.token[i] ;
            }

            sum = sum % 100 ;

            while (1) {
                if ( strcasecmp(table7.at(sum).token.c_str(), "") == 0  ) {
                    temp.place = sum ;
                    temp.value = input.value ;
                    table7.at(sum) = temp ;
                    input.place = sum ;
                    break ;
                }
                else {
                    sum++ ;
                    if ( sum == 100)
                        sum = 0 ;
                }
            }
        } //table7


        void setLocation ( Token & input, int loc) {
            int sum = 0 ;
           // temp.token = input.token ;
            for ( int i = 0 ; i < input.token.size() ; i++ ) {
                sum = sum + input.token[i] ;
            }

            sum = sum % 100 ;

            while (1) {
                if ( strcasecmp(table5.at(sum).token.c_str(), input.token.c_str()) == 0  ) {
                    table5.at(sum).place = loc ; // ???
                    //cout << table5.at(sum).token<< "  LLLLLLLLL  " << loc << endl ;
                    break ;
                }
                else {
                    sum++ ;
                    if ( sum == 100)
                        sum = 0 ;
                }
            }


        }

        bool checkOperand( char input, int &place ) {
            string sTemp = "" ;
            sTemp = sTemp + input ;
            //cout << "test    " << input << endl ;
            for( int i = 0 ; i < table4.size() ; i++ ) {
                if ( strcmp(sTemp.c_str(), table4.at(i).c_str()) == 0 ) {
                    place = i ;
                    return true ;
                }

            }


            return false ;
        } // checkType

        void reset () {
            table1.clear() ;
            table2.clear() ;
            table3.clear() ;
            table4.clear() ;
            table5.clear() ;
            table6.clear() ;
            table7.clear() ;
            tokenTemp.clear() ;
            string nothing = "" ;
            table1.push_back( nothing) ;
            table2.push_back( nothing) ;
            table3.push_back( nothing) ;
            table4.push_back( nothing) ;
            table5.resize( 100 ) ;
            table6.resize( 100 ) ;
            table7.resize( 100 ) ;
            objectCode.clear() ;
            locationTemp.clear() ;
        }

        void print () {
             int temp ;
            int line = 0;
            int location = findStart() ;
            int startLocation = location ;
            //hex << location ;
            string strTemp = "";
            //int location = 0 ;
            cout << "---------------------------------------------------" << endl  ;
            cout << "Line\tLoc\tSource\tstatement\t\tObject code" << endl << endl  ;
            /* for ( int i = 0 ; i < tokenTemp.size() ; i++ ) {
                 for ( int j = 0 ; j < tokenTemp.at(i).size() ; j++ ) {
                        if ( strcmp(tokenTemp.at(i).at(j).token.c_str(), ".") == 0 ) {
                            if ( tokenTemp.at(i).size() == 1 )
                                cout << setw(3) << line << "\t" << location << "\t" << tokenTemp.at(i).at(j).token    ;
                            else {
                                cout << setw(3) << line << "\t" << location << "\t" << tokenTemp.at(i).at(j).token << "\t" << tokenTemp.at(i).at(j+1).token   ;
                                j++;
                            }

                        }
                        else if ( tokenTemp.at(i).at(j).type != 1 &&  tokenTemp.at(i).at(j).type != 2) {
                            cout << setw(3) << line << "\t" << location << "\t" << tokenTemp.at(i).at(j).token << "\t" << tokenTemp.at(i).at(j+1).token << "\t\t"  ;
                            j+=2 ;
                            while ( j < tokenTemp.at(i).size() ) {
                                cout << tokenTemp.at(i).at(j).token ;
                                j++ ;
                            }
                        }
                        else {
                            cout << setw(3) << line << "\t" << location << "\t\t" << tokenTemp.at(i).at(j).token  << "\t\t"  ;
                            j++ ;
                            while ( j < tokenTemp.at(i).size() ) {
                                cout << tokenTemp.at(i).at(j).token ;
                                j++ ;
                            }
                        }

                 } // J

                line+=5;
                 cout << endl ;
            } // I PASS1*/


            for ( int i = 0 ; i < tokenTemp.size() ; i++ ) {
                 for ( int j = 0 ; j < tokenTemp.at(i).size() ; j++ ) {
                        if ( strcmp(tokenTemp.at(i).at(j).token.c_str(), ".") == 0 ) {
                            if ( tokenTemp.at(i).size() == 1 ) {
                               // cout << setw(3) << setfill('0')<< dec << line << "\t    "  << "\t" << tokenTemp.at(i).at(j).token    ;
                                strTemp  ="" ;
                                 objectCode.push_back( strTemp ) ;
                            }
                            else {
                             //   cout << setw(3) << setfill('0')<< dec<< line << "\t    " << "\t" << tokenTemp.at(i).at(j).token << "\t" << tokenTemp.at(i).at(j+1).token   ;
                                strTemp  ="" ;
                                 objectCode.push_back( strTemp ) ;
                                j++;
                            }

                        }
                        else if ( tokenTemp.at(i).at(j).type != 1 &&  tokenTemp.at(i).at(j).type != 2) { //一開始非指令
                            setLocation( tokenTemp.at(i).at(j), location) ;
                            //cout << setw(3) << setfill('0') << dec << line << "\t" << setw(4) << setfill('0') << hex<< location << "\t" << tokenTemp.at(i).at(j).token << "\t" << tokenTemp.at(i).at(j+1).token << "\t"  ;
                            if (  strcasecmp(tokenTemp.at(i).at(j+1).token.c_str(), table2.at(3).c_str()) == 0) {
                                if (  strcasecmp(tokenTemp.at(i).at(j+2).token.c_str(), "X") == 0 ) {
                                    //cout << tokenTemp.at(i).at(j+4).token << endl ;
                                    objectCode.push_back( tokenTemp.at(i).at(j+4).token ) ;
                                    location = location + ( tokenTemp.at(i).at(j+4).token.size()/2) ;
                                } // BYTE X
                                else if (  strcasecmp(tokenTemp.at(i).at(j+2).token.c_str(), "C") == 0 ) {
                                  //  cout << tokenTemp.at(i).at(j+4).token << endl ;
                                   strTemp = cByte( tokenTemp.at(i).at(j+4).token ) ;
                                   //cout << "---------" << strTemp << endl ;
                                   location = location + ( tokenTemp.at(i).at(j+4).token.size()) ;
                                   string strToCode = stringToCode ( tokenTemp.at(i).at(j+4).token ) ;
                                    objectCode.push_back(strToCode ) ;
                                } // BYTE C
                            }
                            else {
                                if (strcasecmp( tokenTemp.at(i).at(j+1).token.c_str(), "START") == 0 ) ;
                                else if (strcasecmp( tokenTemp.at(i).at(j+1).token.c_str(), "END") == 0 ) ;
                                else if ( tokenTemp.at(i).at(j+1).type == 1 )
                                    location+=3 ;
                                else if ( strcasecmp( tokenTemp.at(i).at(j+1).token.c_str(), "RESB") == 0)
                                    location += startLocation ;
                                else if ( tokenTemp.at(i).at(j+1).type == 2 )
                                    location+=3 ;
                            }
                            j+=2 ;
                            while ( j < tokenTemp.at(i).size() ) {
                               // cout << tokenTemp.at(i).at(j).token ;
                                j++ ;
                            }

                           // if ( tokenTemp.at(i).at(j-2).type == 1 )
                              //  cout << "\t\t"<<checkCode(  tokenTemp.at(i).at(j-2))  ;
                              if (strcasecmp( tokenTemp.at(i).at(j-2).token.c_str(), "START") == 0 ) {
                                strTemp  ="" ;
                                 objectCode.push_back( strTemp ) ;
                              }
                              else if ( tokenTemp.at(i).at(j-2).type == 1 ) {
                                strTemp  =checkCode(  tokenTemp.at(i).at(j-2)) ;
                                 objectCode.push_back( strTemp ) ;
                              //  cout << "\t\t"<<strTemp  ;
                            }
                            else if ( tokenTemp.at(i).at(j-2).type == 2 ){
                                strTemp  ="" ;
                                 objectCode.push_back( strTemp ) ;
                               // cout << "\t\t"<<strTemp  ;
                            }

                        }
                        else { //第一割token極為指令
                          //  cout << setw(3) << setfill('0')<< dec <<line << "\t" << setw(4) << setfill('0') << hex << location << "\t\t" << tokenTemp.at(i).at(j).token  << "\t"  ;
                            int temp = j ;
                            j++ ;
                            while ( j < tokenTemp.at(i).size() ) {
                               // cout << tokenTemp.at(i).at(j).token ;
                                j++ ;
                            }

                            if ( tokenTemp.at(i).at(temp).type == 1 ) {
                                strTemp  =checkCode(  tokenTemp.at(i).at(temp)) ;
                                 objectCode.push_back( strTemp ) ;
                               // cout << "\t\t"<<strTemp  ;
                            }
                            else if ( tokenTemp.at(i).at(temp).type == 2 ){
                                strTemp  ="" ;
                                 objectCode.push_back( strTemp ) ;
                                //cout << "\t\t"<<strTemp  ;
                            }


                            if ( tokenTemp.at(i).at(temp).type == 1 )
                                location+=3;
                        }

                 } // J

                //line+=5;
               //  cout << endl ;
            } // I PASS1


            strTemp  ="" ;
            objectCode.push_back( strTemp ) ;
           objectCode.push_back( strTemp ) ;
           // cout << objectCode.size() << endl;
            //for ( int q =0 ; q < objectCode.size() ; q++)
               // cout << q << "     "  << objectCode.at(q) << endl ;
            location = startLocation ;
            int lineCount = 0 ;
            for ( int i = 1 ; i < tokenTemp.size() ; i++ ) {
                 for ( int j = 0 ; j < tokenTemp.at(i).size() ; j++ ) {
                        if ( strcmp(tokenTemp.at(i).at(j).token.c_str(), ".") == 0 ) {
                            if ( tokenTemp.at(i).size() == 1 ) {
                                cout << setw(3) << setfill('0')<< dec << line << "\t    "  << "\t" << tokenTemp.at(i).at(j).token    ;
                            }
                            else {
                                cout << setw(3) << setfill('0')<< dec<< line << "\t    " << "\t" << tokenTemp.at(i).at(j).token << "\t" << tokenTemp.at(i).at(j+1).token   ;
                                j++;
                            }

                            cout << "\t\t" << objectCode.at(lineCount) ;
                        }
                        else if ( tokenTemp.at(i).at(j).type != 1 &&  tokenTemp.at(i).at(j).type != 2) { //一開始非指令
                            cout << setw(3) << setfill('0') << dec << line << "\t" << setw(4) << setfill('0') << setiosflags( ios::uppercase) << hex<< location << "\t" << tokenTemp.at(i).at(j).token << "\t" << tokenTemp.at(i).at(j+1).token << "\t"  ;
                            if (  strcasecmp(tokenTemp.at(i).at(j+1).token.c_str(), table2.at(3).c_str()) == 0) {
                                if (  strcasecmp(tokenTemp.at(i).at(j+2).token.c_str(), "X") == 0 ) {
                                    //cout << tokenTemp.at(i).at(j+4).token << endl ;
                                    location = location + ( tokenTemp.at(i).at(j+4).token.size()/2) ;
                                } // BYTE X
                                else if (  strcasecmp(tokenTemp.at(i).at(j+2).token.c_str(), "C") == 0 ) {
                                  //  cout << tokenTemp.at(i).at(j+4).token << endl ;
                                  location = location + ( tokenTemp.at(i).at(j+4).token.size()) ;
                                } // BYTE C
                            }
                            else {
                                if (strcasecmp( tokenTemp.at(i).at(j+1).token.c_str(), "START") == 0 ) ;
                                else if (strcasecmp( tokenTemp.at(i).at(j+1).token.c_str(), "END") == 0 ) ;
                                else if (strcasecmp( tokenTemp.at(i).at(j+1).token.c_str(), "RESW") == 0 )
                                    location+=3 ;
                                else if (strcasecmp( tokenTemp.at(i).at(j+1).token.c_str(), "BYTE") == 0 ) ;
                                else if (strcasecmp( tokenTemp.at(i).at(j+1).token.c_str(), "RSUB") == 0 )
                                    cout << "0000" ;
                                else if ( tokenTemp.at(i).at(j+1).type == 1 ) {
                                    location+=3 ;
                                }
                                else if ( strcasecmp( tokenTemp.at(i).at(j+1).token.c_str(), "RESB") == 0)
                                    location += startLocation ;
                                else if ( tokenTemp.at(i).at(j+1).type == 2 )
                                    location+=3 ;
                            }
                            temp = j ;
                            j+=2 ;
                            while ( j < tokenTemp.at(i).size() ) {
                                cout << tokenTemp.at(i).at(j).token ;
                                j++ ;
                            }


                            cout << "\t\t" << objectCode.at(lineCount) ;
                            if ( tokenTemp.at(i).at(temp+1).type == 1 ) {
                                for ( int p = 0 ; p < table5.size() ; p++ ) {
                                        if ( strcasecmp( tokenTemp.at(i).at(temp+2).token.c_str(), table5.at(p).token.c_str()) == 0)
                                            cout <<   table5.at(p).place ;
                                    }
                            }
                            else  if (strcasecmp( tokenTemp.at(i).at(temp+1).token.c_str(), "WORD") == 0 ) {
                                    string wordOutput = wordToCode(  tokenTemp.at(i).at(temp+2).token ) ;
                                    cout << wordOutput ;
                            }
                        }
                        else {
                            if (strcasecmp( tokenTemp.at(i).at(j).token.c_str(), "END") == 0 )
                                cout << setw(3) << setfill('0')<< dec <<line << "\t"  << "    \t\t" << tokenTemp.at(i).at(j).token  << "\t"  ;
                            else
                                cout << setw(3) << setfill('0')<< dec <<line << "\t" << setw(4) << setfill('0') << setiosflags( ios::uppercase) << hex << location << "\t\t" << tokenTemp.at(i).at(j).token  << "\t"  ;
                            temp = j ;
                            j++ ;
                            while ( j < tokenTemp.at(i).size() ) {
                                cout << tokenTemp.at(i).at(j).token ;
                                j++ ;
                            }

                            if ( tokenTemp.at(i).at(temp).type == 1 )
                                location+=3;

                            cout << "\t\t" << objectCode.at(lineCount) ;
                            if (strcasecmp( tokenTemp.at(i).at(temp).token.c_str(), "STCH") == 0 || strcasecmp( tokenTemp.at(i).at(temp).token.c_str(), "LDCH") == 0) {
                                if ( tokenTemp.at(i).size() >= 4 ) {
                                    if ( strcasecmp( tokenTemp.at(i).at(temp+2).token.c_str(), ",") == 0) {
                                        if ( tokenTemp.at(i).at(temp+3).type == 3 ) {
                                             for ( int p = 0 ; p < table5.size() ; p++ ) {
                                                if ( strcasecmp( tokenTemp.at(i).at(temp+1).token.c_str(), table5.at(p).token.c_str()) == 0) {
                                                    int temp = table5.at(p).place +32768;
                                                    cout <<   temp ;
                                                    break ;
                                                }
                                            }
                                        }
                                    }
                                }
                                else {
                                    cout << " Erro" ;
                                }
                            }
                            else if (strcasecmp( tokenTemp.at(i).at(temp).token.c_str(), "RSUB") == 0 )
                                    cout << "0000" ;
                            else  if (strcasecmp( tokenTemp.at(i).at(temp).token.c_str(), "END") == 0 ) ;
                            else {
                                    for ( int p = 0 ; p < table5.size() ; p++ ) {
                                        if ( strcasecmp( tokenTemp.at(i).at(temp+1).token.c_str(), table5.at(p).token.c_str()) == 0)
                                            cout <<   table5.at(p).place ;
                                    }
                            }

                        }

                 } // J



                 lineCount++ ;
                line+=5;
                 cout << endl ;
            } // I PASS2

        }


        void write () {
             int temp ;
            int line = 5;
            int location = findStart() ;
            int startLocation = location ;
            string strTemp = "";
            newFile.open("My"+ fileName + "_output.txt", ios::out);
            newFile << "Line\tLoc\tSource\tstatement\t\tObject code" << endl << endl  ;
            for ( int i = 0 ; i < tokenTemp.size() ; i++ ) {
                 for ( int j = 0 ; j < tokenTemp.at(i).size() ; j++ ) {
                        if ( strcmp(tokenTemp.at(i).at(j).token.c_str(), ".") == 0 ) {
                            if ( tokenTemp.at(i).size() == 1 ) {
                               // cout << setw(3) << setfill('0')<< dec << line << "\t    "  << "\t" << tokenTemp.at(i).at(j).token    ;
                                strTemp  ="" ;
                                 objectCode.push_back( strTemp ) ;
                            }
                            else {
                             //   cout << setw(3) << setfill('0')<< dec<< line << "\t    " << "\t" << tokenTemp.at(i).at(j).token << "\t" << tokenTemp.at(i).at(j+1).token   ;
                                strTemp  ="" ;
                                 objectCode.push_back( strTemp ) ;
                                j++;
                            }

                        }
                        else if ( tokenTemp.at(i).at(j).type != 1 &&  tokenTemp.at(i).at(j).type != 2) { //一開始非指令
                            setLocation( tokenTemp.at(i).at(j), location) ;
                            //cout << setw(3) << setfill('0') << dec << line << "\t" << setw(4) << setfill('0') << hex<< location << "\t" << tokenTemp.at(i).at(j).token << "\t" << tokenTemp.at(i).at(j+1).token << "\t"  ;
                            if (  strcasecmp(tokenTemp.at(i).at(j+1).token.c_str(), table2.at(3).c_str()) == 0) {
                                if (  strcasecmp(tokenTemp.at(i).at(j+2).token.c_str(), "X") == 0 ) {
                                    //cout << tokenTemp.at(i).at(j+4).token << endl ;
                                    objectCode.push_back( tokenTemp.at(i).at(j+4).token ) ;
                                    location = location + ( tokenTemp.at(i).at(j+4).token.size()/2) ;
                                } // BYTE X
                                else if (  strcasecmp(tokenTemp.at(i).at(j+2).token.c_str(), "C") == 0 ) {
                                  //  cout << tokenTemp.at(i).at(j+4).token << endl ;
                                   strTemp = cByte( tokenTemp.at(i).at(j+4).token ) ;
                                   //cout << "---------" << strTemp << endl ;
                                   location = location + ( tokenTemp.at(i).at(j+4).token.size()) ;
                                   string strToCode = stringToCode ( tokenTemp.at(i).at(j+4).token ) ;
                                    objectCode.push_back(strToCode ) ;
                                } // BYTE C
                            }
                            else {
                                if (strcasecmp( tokenTemp.at(i).at(j+1).token.c_str(), "START") == 0 ) ;
                                else if (strcasecmp( tokenTemp.at(i).at(j+1).token.c_str(), "END") == 0 ) ;
                                else if ( tokenTemp.at(i).at(j+1).type == 1 )
                                    location+=3 ;
                                else if ( strcasecmp( tokenTemp.at(i).at(j+1).token.c_str(), "RESB") == 0)
                                    location += startLocation ;
                                else if ( tokenTemp.at(i).at(j+1).type == 2 )
                                    location+=3 ;
                            }
                            j+=2 ;
                            while ( j < tokenTemp.at(i).size() ) {
                               // cout << tokenTemp.at(i).at(j).token ;
                                j++ ;
                            }

                           // if ( tokenTemp.at(i).at(j-2).type == 1 )
                              //  cout << "\t\t"<<checkCode(  tokenTemp.at(i).at(j-2))  ;
                              if (strcasecmp( tokenTemp.at(i).at(j-2).token.c_str(), "START") == 0 ) {
                                strTemp  ="" ;
                                 objectCode.push_back( strTemp ) ;
                              }
                              else if ( tokenTemp.at(i).at(j-2).type == 1 ) {
                                strTemp  =checkCode(  tokenTemp.at(i).at(j-2)) ;
                                 objectCode.push_back( strTemp ) ;
                              //  cout << "\t\t"<<strTemp  ;
                            }
                            else if ( tokenTemp.at(i).at(j-2).type == 2 ){
                                strTemp  ="" ;
                                 objectCode.push_back( strTemp ) ;
                               // cout << "\t\t"<<strTemp  ;
                            }

                        }
                        else { //第一割token極為指令
                          //  cout << setw(3) << setfill('0')<< dec <<line << "\t" << setw(4) << setfill('0') << hex << location << "\t\t" << tokenTemp.at(i).at(j).token  << "\t"  ;
                            int temp = j ;
                            j++ ;
                            while ( j < tokenTemp.at(i).size() ) {
                               // cout << tokenTemp.at(i).at(j).token ;
                                j++ ;
                            }

                            if ( tokenTemp.at(i).at(temp).type == 1 ) {
                                strTemp  =checkCode(  tokenTemp.at(i).at(temp)) ;
                                 objectCode.push_back( strTemp ) ;
                               // cout << "\t\t"<<strTemp  ;
                            }
                            else if ( tokenTemp.at(i).at(temp).type == 2 ){
                                strTemp  ="" ;
                                 objectCode.push_back( strTemp ) ;
                                //cout << "\t\t"<<strTemp  ;
                            }


                            if ( tokenTemp.at(i).at(temp).type == 1 )
                                location+=3;
                        }

                 } // J

                //line+=5;
               //  cout << endl ;
            } // I PASS1


            strTemp  ="" ;
            objectCode.push_back( strTemp ) ;
           objectCode.push_back( strTemp ) ;
           // cout << objectCode.size() << endl;
            //for ( int q =0 ; q < objectCode.size() ; q++)
               // cout << q << "     "  << objectCode.at(q) << endl ;
            location = startLocation ;
            int lineCount = 0 ;
            for ( int i = 1 ; i < tokenTemp.size() ; i++ ) {
                 for ( int j = 0 ; j < tokenTemp.at(i).size() ; j++ ) {
                        if ( strcmp(tokenTemp.at(i).at(j).token.c_str(), ".") == 0 ) {
                            if ( tokenTemp.at(i).size() == 1 ) {
                                newFile << setw(3) << setfill(' ')<< dec << line << "\t    "  << "\t" << tokenTemp.at(i).at(j).token    ;
                            }
                            else {
                                newFile << setw(3) << setfill(' ')<< dec<< line << "\t    " << "\t" << tokenTemp.at(i).at(j).token << "\t" << tokenTemp.at(i).at(j+1).token   ;
                                j++;
                            }

                            newFile << "\t\t" << objectCode.at(lineCount) ;
                        }
                        else if ( tokenTemp.at(i).at(j).type != 1 &&  tokenTemp.at(i).at(j).type != 2) { //一開始非指令
                            newFile << setw(3) << setfill(' ') << dec << line << "\t" << setw(4) << setfill('0') << setiosflags( ios::uppercase) << hex<< location << "\t" << tokenTemp.at(i).at(j).token << "\t" << tokenTemp.at(i).at(j+1).token << "\t"  ;
                            if (  strcasecmp(tokenTemp.at(i).at(j+1).token.c_str(), table2.at(3).c_str()) == 0) {
                                if (  strcasecmp(tokenTemp.at(i).at(j+2).token.c_str(), "X") == 0 ) {
                                    //cout << tokenTemp.at(i).at(j+4).token << endl ;
                                    location = location + ( tokenTemp.at(i).at(j+4).token.size()/2) ;
                                } // BYTE X
                                else if (  strcasecmp(tokenTemp.at(i).at(j+2).token.c_str(), "C") == 0 ) {
                                  //  cout << tokenTemp.at(i).at(j+4).token << endl ;
                                  location = location + ( tokenTemp.at(i).at(j+4).token.size()) ;
                                } // BYTE C
                            }
                            else {
                                if (strcasecmp( tokenTemp.at(i).at(j+1).token.c_str(), "START") == 0 ) ;
                                else if (strcasecmp( tokenTemp.at(i).at(j+1).token.c_str(), "END") == 0 ) ;
                                else if (strcasecmp( tokenTemp.at(i).at(j+1).token.c_str(), "RESW") == 0 )
                                    location+=3 ;
                                else if (strcasecmp( tokenTemp.at(i).at(j+1).token.c_str(), "BYTE") == 0 ) ;
                                else if (strcasecmp( tokenTemp.at(i).at(j+1).token.c_str(), "RSUB") == 0 )
                                    newFile << "0000" ;
                                else if ( tokenTemp.at(i).at(j+1).type == 1 ) {
                                    location+=3 ;
                                }
                                else if ( strcasecmp( tokenTemp.at(i).at(j+1).token.c_str(), "RESB") == 0)
                                    location += startLocation ;
                                else if ( tokenTemp.at(i).at(j+1).type == 2 )
                                    location+=3 ;
                            }
                            temp = j ;
                            j+=2 ;
                            while ( j < tokenTemp.at(i).size() ) {
                                newFile << tokenTemp.at(i).at(j).token ;
                                j++ ;
                            }


                            newFile << "\t\t" << objectCode.at(lineCount) ;
                            if ( tokenTemp.at(i).at(temp+1).type == 1 ) {
                                for ( int p = 0 ; p < table5.size() ; p++ ) {
                                        if ( strcasecmp( tokenTemp.at(i).at(temp+2).token.c_str(), table5.at(p).token.c_str()) == 0)
                                            newFile <<   table5.at(p).place ;
                                    }
                            }
                            else  if (strcasecmp( tokenTemp.at(i).at(temp+1).token.c_str(), "WORD") == 0 ) {
                                    string wordOutput = wordToCode(  tokenTemp.at(i).at(temp+2).token ) ;
                                    newFile << wordOutput ;
                            }
                        }
                        else {
                            if (strcasecmp( tokenTemp.at(i).at(j).token.c_str(), "END") == 0 )
                                newFile << setw(3) << setfill(' ')<< dec <<line << "\t"  << "    \t\t" << tokenTemp.at(i).at(j).token  << "\t"  ;
                            else
                                newFile << setw(3) << setfill(' ')<< dec <<line << "\t" << setw(4) << setfill('0') << setiosflags( ios::uppercase) << hex << location << "\t\t" << tokenTemp.at(i).at(j).token  << "\t"  ;
                            temp = j ;
                            j++ ;
                            while ( j < tokenTemp.at(i).size() ) {
                                newFile << tokenTemp.at(i).at(j).token ;
                                j++ ;
                            }

                            if ( tokenTemp.at(i).at(temp).type == 1 )
                                location+=3;

                            newFile << "\t\t" << objectCode.at(lineCount) ;
                            if (strcasecmp( tokenTemp.at(i).at(temp).token.c_str(), "STCH") == 0 || strcasecmp( tokenTemp.at(i).at(temp).token.c_str(), "LDCH") == 0) {
                                if ( tokenTemp.at(i).size() >= 4 ) {
                                    if ( strcasecmp( tokenTemp.at(i).at(temp+2).token.c_str(), ",") == 0) {
                                        if ( tokenTemp.at(i).at(temp+3).type == 3 ) {
                                             for ( int p = 0 ; p < table5.size() ; p++ ) {
                                                if ( strcasecmp( tokenTemp.at(i).at(temp+1).token.c_str(), table5.at(p).token.c_str()) == 0) {
                                                    int temp = table5.at(p).place +32768;
                                                    newFile <<   temp ;
                                                    break ;
                                                }
                                            }
                                        }
                                    }
                                }
                                else {
                                    newFile << " Erro" ;
                                }
                            }
                            else if (strcasecmp( tokenTemp.at(i).at(temp).token.c_str(), "RSUB") == 0 )
                                    newFile << "0000" ;
                            else  if (strcasecmp( tokenTemp.at(i).at(temp).token.c_str(), "END") == 0 ) ;
                            else {
                                    for ( int p = 0 ; p < table5.size() ; p++ ) {
                                        if ( strcasecmp( tokenTemp.at(i).at(temp+1).token.c_str(), table5.at(p).token.c_str()) == 0)
                                            newFile <<   table5.at(p).place ;
                                    }
                            }

                        }

                 } // J



                 lineCount++ ;
                line+=5;
                 newFile << endl ;
            } // I PASS2

             newFile.close();
        }

        string wordToCode ( string input ) {
            //cout << "---------" ;
            int sum = 0 ;
            string result = "" ;
            int count = 0 ;
            for ( int i = 0 ; i < input.size() ; i ++) {
                sum *= 10 ;
                sum = sum + ( input[i] - '0' ) ;
            }


           // cout << endl << sum << endl ;
            int max = 16 ;
            int temp = 1 ;
     //       if ( sum < 16777216){
        //        result = result + "0" ;
            //}
           if ( sum < 1048576) {
                result = result + "0" ;
                max = 1048576 ;
                count ++ ;
            }
            if ( sum < 65536 ) {
                result = result + "0" ;
                max = 65536 ;
                count ++ ;
            }
            if ( sum < 4096) {
                result = result + "0" ;
                max = 4096 ;
                count ++ ;
            }
            if ( sum < 256 ) {
                result = result + "0" ;
                max = 256 ;
                count ++ ;
            }
            if ( sum < 16 ) {
                result = result + "0" ;
                max = 16 ;
                count ++ ;
            }
            if ( sum == 0 ) {
                //result = result + "0" ;
                count ++ ;
                max = 1 ;
            }


            while ( sum > 0 )  {
                //cout << "--------***-" << dec << temp << "       " << max << endl ;
                temp = sum ;
                if ( temp / max == 1 )
                    result = result + "1" ;
                else if ( temp / max == 2 )
                    result = result + "2" ;
                else if ( temp / max == 3 )
                    result = result + "3" ;
                else if ( temp / max == 4 )
                    result = result + "4" ;
                else if ( temp / max == 5 )
                    result = result + "5" ;
                else if ( temp / max == 6 )
                    result = result + "6" ;
                else if ( temp / max == 7 )
                    result = result + "7" ;
                else if ( temp / max == 8 )
                    result = result + "8" ;
                else if ( temp / max == 9 )
                    result = result + "9" ;
                else if ( temp / max == 10 )
                    result = result + "A" ;
                else if ( temp / max == 11 )
                    result = result + "B" ;
                else if ( temp / max == 12 )
                    result = result + "C" ;
                else if ( temp / max == 13 )
                    result = result + "D" ;
                else if ( temp / max == 14 )
                    result = result + "E" ;
                else if ( temp / max == 15 )
                    result = result + "F" ;
                count ++ ;
                sum %= max ;
                max /= 16 ;
            }
            while ( count <= 6 ) {
                result = result + "0" ;
                count ++ ;
            }



            return result ;
        }

        string stringToCode( string input ) {
            string result = "" ;
            //cout << "----------------" << endl ;
            for ( int i = 0 ; i < input.size() ; i ++ ) {
                int temp = input[i] ;
                if ( temp / 16 == 1 )
                    result = result + "1" ;
                else if ( temp / 16 == 2 )
                    result = result + "2" ;
                else if ( temp / 16 == 3 )
                    result = result + "3" ;
                else if ( temp / 16 == 4 )
                    result = result + "4" ;
                else if ( temp / 16 == 5 )
                    result = result + "5" ;
                else if ( temp / 16 == 6 )
                    result = result + "6" ;
                else if ( temp / 16 == 7 )
                    result = result + "7" ;
                else if ( temp / 16 == 8 )
                    result = result + "8" ;
                else if ( temp / 16 == 9 )
                    result = result + "9" ;
                else if ( temp / 16 == 10 )
                    result = result + "A" ;
                else if ( temp / 16 == 11 )
                    result = result + "B" ;
                else if ( temp / 16 == 12 )
                    result = result + "C" ;
                else if ( temp / 16 == 13 )
                    result = result + "D" ;
                else if ( temp / 16 == 14 )
                    result = result + "E" ;
                else if ( temp / 16 == 15 )
                    result = result + "F" ;
                if ( temp % 16 == 1 )
                    result = result + "1" ;
                else if ( temp % 16 == 2 )
                    result = result + "2" ;
                else if ( temp % 16 == 3 )
                    result = result + "3" ;
                else if ( temp % 16 == 4 )
                    result = result + "4" ;
                else if ( temp % 16 == 5 )
                    result = result + "5" ;
                else if ( temp % 16 == 6 )
                    result = result + "6" ;
                else if ( temp % 16 == 7 )
                    result = result + "7" ;
                else if ( temp % 16 == 8 )
                    result = result + "8" ;
                else if ( temp % 16 == 9 )
                    result = result + "9" ;
                else if ( temp % 16 == 10 )
                    result = result + "A" ;
                else if ( temp % 16 == 11 )
                    result = result + "B" ;
                else if ( temp % 16 == 12 )
                    result = result + "C" ;
                else if ( temp %16 == 13 )
                    result = result + "D" ;
                else if ( temp % 16 == 14 )
                    result = result + "E" ;
                else if ( temp %16 == 15 )
                    result = result + "F" ;
            }


            return result ;
        }


        string cByte( string input ) {
            string result ;
            int sum = 0 ;
            int temp ;
            //cout << "test" << endl ;
            for ( int i = 0 ; i < input.size() ; i++ ) {
                sum = input[i]  ;
                temp = sum / 16 ;
                result = result + toHex( temp ) ;
                temp = sum%16 ;
                result = result + toHex( temp ) ;
            }

           // cout << "-------" << result << endl ;

           return result ;
        }

        string toHex ( int input) {
            if ( input == 1 )
                return "1" ;
            else if ( input ==2 )
                return "2" ;
            else if ( input ==3 )
                return "3" ;
            else if ( input ==4 )
                return "4" ;
            else if ( input ==5 )
                return "5" ;
            else if ( input ==6 )
                return "6" ;
            else if ( input ==7 )
                return "7" ;
            else if ( input ==8 )
                return "8" ;
            else if ( input ==9 )
                return "9" ;
            else if ( input ==10 )
                return "A" ;
            else if ( input ==11 )
                return "B" ;
            else if ( input ==12 )
                return "C" ;
            else if ( input ==13 )
                return "D" ;
            else if ( input ==14 )
                return "E" ;
            else if ( input ==15 )
                return "F" ;
        }


        int stringToInt ( string input ) {
            int result = 0 ;
            for ( int i = 0 ; i< input.size() ; i++) {
                result = result * 10 ;
                result = result + ( input[i] - '0' ) ;
            }

            //cout << "88888888     "  << result << endl ;
            return result ;
        }

        string checkCode ( Token input) {
          // input.token = "ADD" ;
             if ( strcasecmp(input.token.c_str(), table1.at(1).c_str()) == 0 ) {
                 return "18" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(2).c_str()) == 0 ) {
                 return "58" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(3).c_str()) == 0 ) {
                 return "90" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(4).c_str()) == 0 ) {
                 return "40" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(5).c_str()) == 0 ) {
                 return "B4" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(6).c_str()) == 0 ) {
                 return "28" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(7).c_str()) == 0 ) {
                 return "88" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(8).c_str()) == 0 ) {
                 return "A0" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(9).c_str()) == 0 ) {
                 return "24" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(10).c_str()) == 0 ) {
                 return "64" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(11).c_str()) == 0 ) {
                 return "9C" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(12).c_str()) == 0 ) {
                 return "C4" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(13).c_str()) == 0 ) {
                 return "C0" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(14).c_str()) == 0 ) {
                 return "F4" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(15).c_str()) == 0 ) {
                 return "3C" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(16).c_str()) == 0 ) {
                 return "30" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(17).c_str()) == 0 ) {
                 return "34" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(18).c_str()) == 0 ) {
                 return "38" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(19).c_str()) == 0 ) {
                 return "48" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(20).c_str()) == 0 ) {
                 return "00" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(21).c_str()) == 0 ) {
                 return "68" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(22).c_str()) == 0 ) {
                 return "50" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(23).c_str()) == 0 ) {
                 return "70" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(24).c_str()) == 0 ) {
                 return "08" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(25).c_str()) == 0 ) {
                 return "6C" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(26).c_str()) == 0 ) {
                 return "74" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(27).c_str()) == 0 ) {
                 return "04" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(28).c_str()) == 0 ) {
                 return "D0" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(29).c_str()) == 0 ) {
                 return "20" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(30).c_str()) == 0 ) {
                 return "60" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(31).c_str()) == 0 ) {
                 return "98" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(32).c_str()) == 0 ) {
                 return "C8" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(33).c_str()) == 0 ) {
                 return "44" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(34).c_str()) == 0 ) {
                 return "D8" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(35).c_str()) == 0 ) {
                 return "AC" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(36).c_str()) == 0 ) {
                 return "4C" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(37).c_str()) == 0 ) {
                 return "A4" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(38).c_str()) == 0 ) {
                 return "A8" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(39).c_str()) == 0 ) {
                 return "F0" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(40).c_str()) == 0 ) {
                 return "EC" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(41).c_str()) == 0 ) {
                 return "0C" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(42).c_str()) == 0 ) {
                 return "78" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(43).c_str()) == 0 ) {
                 return "54" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(44).c_str()) == 0 ) {
                 return "80" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(45).c_str()) == 0 ) {
                 return "D4" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(46).c_str()) == 0 ) {
                 return "14" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(47).c_str()) == 0 ) {
                 return "7C" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(48).c_str()) == 0 ) {
                 return "E8" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(49).c_str()) == 0 ) {
                 return "84" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(50).c_str()) == 0 ) {
                 return "10" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(51).c_str()) == 0 ) {
                 return "1C" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(52).c_str()) == 0 ) {
                 return "5C" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(53).c_str()) == 0 ) {
                 return "94" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(54).c_str()) == 0 ) {
                 return "B0" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(55).c_str()) == 0 ) {
                 return "E0" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(56).c_str()) == 0 ) {
                 return "F8" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(57).c_str()) == 0 ) {
                 return "2C" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(58).c_str()) == 0 ) {
                 return "B8" ;
             }
             else if ( strcasecmp(input.token.c_str(), table1.at(59).c_str()) == 0 ) {
                 return "DC" ;
             }

             return "99999";
        }


        int findStart () {
            int result = 0 ;
            for ( int i = 0 ; i < tokenTemp.size() ; i++ ) {
                 for ( int j = 0 ; j < tokenTemp.at(i).size() ; j++ ) {
                        if ( strcasecmp(tokenTemp.at(i).at(j).token.c_str(), "START") == 0 ) {
                            for ( int k = 0 ; k < tokenTemp.at(i).at(j+1).token.size() ; k++) {
                                result*=16 ;
                                result = result + ( tokenTemp.at(i).at(j+1).token[k] - '0' ) ;
                               /* cout << tokenTemp.at(i).at(j+1).token[k] << endl;
                            cout << tokenTemp.at(i).at(j+1).token.size() << endl;
                                cout << result << endl;*/
                            }
                            return result ;
                        }
                 }
            }
        }


        bool checkNoCode( string input ){
            if (  strcasecmp(input.c_str(), "START") == 0 )
                return true ;
            else if (  strcasecmp(input.c_str(), "END") == 0 )
                return true ;
            else if (  strcasecmp(input.c_str(), "RESB") == 0 )
                return true ;
            else if (  strcasecmp(input.c_str(), "RESW") == 0 )
                return true ;
            else if (  strcasecmp(input.c_str(), "EQU") == 0 )
                return true ;
            else if (  strcasecmp(input.c_str(), "BASE") == 0 )
                return true ;
            else if (  strcasecmp(input.c_str(), "LTORG") == 0 )
                return true ;
            return false ;

        }

        int checkFormat( string input ) {
             if ( strcasecmp(input.c_str(), table1.at(1).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(2).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(3).c_str()) == 0 ) {
                 return 2 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(4).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(5).c_str()) == 0 ) {
                 return 2 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(6).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(7).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(8).c_str()) == 0 ) {
                 return 2 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(9).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(10).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(11).c_str()) == 0 ) {
                 return 2 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(12).c_str()) == 0 ) {
                 return 1 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(13).c_str()) == 0 ) {
                 return 1 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(14).c_str()) == 0 ) {
                 return 1 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(15).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(16).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(17).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(18).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(19).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(20).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(21).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(22).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(23).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(24).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(25).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(26).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(27).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(28).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(29).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(30).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(31).c_str()) == 0 ) {
                 return 2 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(32).c_str()) == 0 ) {
                 return 1 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(33).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(34).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(35).c_str()) == 0 ) {
                 return 2 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(36).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(37).c_str()) == 0 ) {
                 return 2 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(38).c_str()) == 0 ) {
                 return 2 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(39).c_str()) == 0 ) {
                 return 1 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(40).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(41).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(42).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(43).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(44).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(45).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(46).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(47).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(48).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(49).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(50).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(51).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(52).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(53).c_str()) == 0 ) {
                 return 2 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(54).c_str()) == 0 ) {
                 return 2 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(55).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(56).c_str()) == 0 ) {
                 return 1 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(57).c_str()) == 0 ) {
                 return 3 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(58).c_str()) == 0 ) {
                 return 2 ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(59).c_str()) == 0 ) {
                 return 3 ;
             }

             return 0;
        }

        int checkLine ( vector < Token > input, int & space ) { //format 1~4  pseudo 5 ( no loc = 9 )comment 6 = 7 EQU 8
                    if ( input.at(0).type == 5 && input.size() != 1 ) { // 為symbol
                        if ( strcasecmp(input[2].token.c_str(),"=") == 0) {
                            if ( input.size() == 7 ) {
                                if ( input[1].type == 1  && strcasecmp(input[2].token.c_str(),"=") == 0 && ( strcasecmp(input[3].token.c_str(),"C") == 0 ||   strcasecmp(input[3].token.c_str(),"X") == 0 ) && strcasecmp(input[4].token.c_str(),"\'") == 0&& strcasecmp(input[6].token.c_str(),"\'") == 0 ) {
                                    space = checkFormat( input.at(1).token) ;
                                    return 7 ;
                                }
                                else
                                    return 0 ;
                             }
                        }
                        else if (  strcasecmp(input[1].token.c_str(),"START") == 0  && input.size() == 3 ) {
                            if (  input.at(2).type == 6) {
                                space = 0 ;
                                return 5 ;
                            }
                            else
                                return 0 ;
                        }
                        else if (  strcasecmp(input[1].token.c_str(),"EQU") == 0  && input.size() == 3 ) {
                            if (  input.at(2).type == 6 || input.at(2).type == 5 || strcasecmp(input[2].token.c_str(),"*") == 0 ){
                                if ( input.at(2).type == 6 )
                                    space = stringToInt( input.at(2).token) ;
                                return 8 ;
                            } //不動{
                            else
                                return 0 ;
                        }
                        else if (  strcasecmp(input[1].token.c_str(),"END") == 0  && input.size() == 3 ) {
                            if (  input.at(2).type == 6) {
                                space = 0 ;
                                return 9 ;
                            }
                            else
                                return 0 ;
                        }
                        else if (  strcasecmp(input[1].token.c_str(),"END") == 0  && input.size() == 2 ) {
                            space = 0 ;
                            return 9 ;
                        }
                        else if (  strcasecmp(input[1].token.c_str(),"BYTE") == 0   && input.size() == 3 ) { //byte with symbol
                            if (  input.at(2).type == 6) {
                                space = 3 ;
                                return 5 ;
                            }
                            else
                                return 0 ;
                        }
                        else if (  strcasecmp(input[1].token.c_str(),"BYTE") == 0   && input.size() == 6 ) { // byte with c and x
                            if (  strcasecmp(input[2].token.c_str(),"C") == 0  &&   strcasecmp(input[3].token.c_str(),"\'") == 0 &&  input.at(4).type == 7 &&  strcasecmp(input[5].token.c_str(),"\'") == 0) {
                                string temp = stringToCode( input.at(4).token) ;
                                space  =  temp.size() /2 ;
                                return 5 ;
                            }
                            else if ( strcasecmp(input[2].token.c_str(),"X") == 0  &&   strcasecmp(input[3].token.c_str(),"\'") == 0 &&  input.at(4).type == 6 &&  strcasecmp(input[5].token.c_str(),"\'") == 0 ) {
                                space = input.at(4).token.size() / 2 ;
                                return 5 ;
                            }
                            else
                                return 0 ;
                        }
                        else if (  strcasecmp(input[1].token.c_str(),"WORD") == 0   && input.size() == 6 ) {
                            if (  strcasecmp(input[2].token.c_str(),"C") == 0  &&   strcasecmp(input[3].token.c_str(),"\'") == 0 &&  input.at(4).type == 7 &&  strcasecmp(input[5].token.c_str(),"\'") == 0) {
                                space = 3 ;
                                return 5 ;
                            }
                            else if ( strcasecmp(input[2].token.c_str(),"X") == 0  &&   strcasecmp(input[3].token.c_str(),"\'") == 0 &&  input.at(4).type == 6 &&  strcasecmp(input[5].token.c_str(),"\'") == 0 ) {
                                space = 3 ;
                                return 5 ;
                            }
                            else
                                return 0 ;
                        }
                        else if (  strcasecmp(input[1].token.c_str(),"LTORG") == 0  && input.size() == 2 ) {
                            space = 0 ;
                            return 9 ;
                        }
                        else if (  strcasecmp(input[1].token.c_str(),"BASE") == 0  && input.size() == 3 ) {
                            if (  input.at(2).type == 6 || input.at(2).type == 5 ) {
                                space = 0 ;
                                return 9 ;
                            }
                            else
                                return 0 ;
                        }
                        else if (  strcasecmp(input[1].token.c_str(),"RESB") == 0  && input.size() == 3 ) {
                            if (  input.at(2).type == 6  ) {
                                space = stringToInt( input.at(2).token) ;
                                return 5 ;
                            }
                            else
                                return 0 ;
                        }
                        else if (  strcasecmp(input[1].token.c_str(),"RESW") == 0  && input.size() == 3 ) {
                            if (  input.at(2).type == 6  ) {
                                space = stringToInt( input.at(2).token) *3;
                                return 5 ;
                            }
                            else
                                return 0 ;
                        }
                        else if ( input.at(1).type == 1) {
                            if ( input.size() == 2 ) {
                                   if (strcasecmp(input[1].token.c_str(),"RSUB") ==0 )
                                        space = 3 ;
                                    else
                                        space = 1 ;
                                    return 1 ;
                             }
                             else if ( input.size() == 3 ) {
                                if (input[1].type == 1  && (input[2].type == 5|| input[2].type == 6 || input[2].type == 3) ) {
                                    if (strcasecmp(input[1].token.c_str(),"CLEAR") ==0 || strcasecmp(input[1].token.c_str(),"TIXR") == 0)
                                        space = 2 ;
                                    else
                                        space = 3 ;
                                    return 1 ;
                                }
                                else
                                    return 0 ;
                             }
                             if ( input.size() == 4 ) {
                                if ( input[1].type == 1  &&( strcasecmp(input[2].token.c_str(),"#") == 0 || strcasecmp(input[2].token.c_str(),"@") == 0 )  && (input[3].type == 5|| input[3].type == 6) ) {
                                    space = 3 ;
                                    return 1 ;
                                }
                                else
                                    return 0 ;
                             }
                            else if ( input.size() == 5 ) {
                                if ( input[1].type == 1  &&(input[2].type == 5|| input[2].type == 6 || input[2].type == 3)  && (input[4].type == 5|| input[4].type == 6|| input[4].type == 3) && strcasecmp(input[3].token.c_str(),",") == 0 ) {
                                    if (strcasecmp(input[1].token.c_str(),"STCH") ==0 || strcasecmp(input[1].token.c_str(),"LDCH") ==0)
                                        space = 3 ;
                                    else
                                        space = 2 ;
                                    return 1 ;
                                }
                                else
                                    return 0 ;
                             }
                        }
                        else if ( strcasecmp(input[1].token.c_str(),"+") == 0) {
                            space = 4 ;
                            if ( input.size() == 3 ) {
                                if ( input[2].type == 1 )
                                    return 1 ;
                                else
                                    return 0 ;
                             }
                             else if ( input.size() == 4 ) {
                                if (input[2].type == 1  && (input[3].type == 5|| input[3].type == 6 || input[3].type == 3) )
                                    return 1 ;
                                else
                                    return 0 ;
                             }
                             if ( input.size() == 5 ) {
                                if ( input[2].type == 1  &&( strcasecmp(input[3].token.c_str(),"#") == 0 || strcasecmp(input[3].token.c_str(),"@") == 0 )  && (input[4].type == 5|| input[4].type == 6) )
                                    return 1 ;
                                else
                                    return 0 ;
                             }
                            else if ( input.size() == 6 ) {
                                if ( input[2].type == 1  &&(input[3].type == 5|| input[3].type == 6 || input[3].type == 3)  && (input[5].type == 5|| input[5].type == 6|| input[5].type == 3) && strcasecmp(input[4].token.c_str(),",") == 0 )
                                    return 1 ;
                                else
                                    return 0 ;
                             }
                        }
                        else {
                            return 0 ;
                        }
                    }
                    else if (  input.at(0).type == 1 ) { // 為instruction
                             if ( input.size() == 1 ) {
                                   if (strcasecmp(input[0].token.c_str(),"RSUB") ==0 )
                                        space = 3 ;
                                    else
                                        space = 1 ;
                                    return 1 ;
                             }
                             else if ( input.size() == 2 ) {
                                if (input[0].type == 1  && (input[1].type == 5|| input[1].type == 6 || input[1].type == 3) ) {
                                    if (strcasecmp(input[0].token.c_str(),"CLEAR") == 0|| strcasecmp(input[0].token.c_str(),"TIXR") == 0)
                                        space = 2 ;
                                    else
                                        space = 3 ;
                                    return 1 ;
                                }
                                else
                                    return 0 ;
                             }
                             if ( input.size() == 3 ) {
                                space = 3 ;
                                if ( input[0].type == 1  &&( strcasecmp(input[1].token.c_str(),"#") == 0 || strcasecmp(input[1].token.c_str(),"@") == 0 )  && (input[2].type == 5|| input[2].type == 6) )
                                    return 1 ;
                                else
                                    return 0 ;
                             }
                            else if ( input.size() == 4 ) {
                                if (strcasecmp(input[0].token.c_str(),"STCH") ==0 || strcasecmp(input[0].token.c_str(),"LDCH") ==0)
                                        space = 3 ;
                                else
                                        space = 2 ;
                                if ( input[0].type == 1  &&(input[1].type == 5|| input[1].type == 6 || input[1].type == 3)  && (input[3].type == 5|| input[3].type == 6|| input[3].type == 3) && strcasecmp(input[2].token.c_str(),",") == 0 )
                                    return 1 ;
                                else
                                    return 0 ;
                             }
                            else if ( strcasecmp(input[1].token.c_str(),"=") == 0) {
                                space = 3 ;
                                if ( input.size() == 6 ) {
                                    if ( input[0].type == 1  && strcasecmp(input[1].token.c_str(),"=") == 0 && ( strcasecmp(input[2].token.c_str(),"C") == 0 ||   strcasecmp(input[2].token.c_str(),"X") == 0 ) && strcasecmp(input[3].token.c_str(),"\'") == 0&& strcasecmp(input[5].token.c_str(),"\'") == 0 )
                                        return 7 ;
                                    else
                                        return 0 ;
                                }
                            }
                    }
                    else if (  input.at(0).type ==  2) { // 為P instruction
                        if (  strcasecmp(input[0].token.c_str(),"START") == 0  && input.size() == 2 ) {
                            space = 0 ;
                            if (  input.at(1).type == 6)
                                return 9 ;
                            else
                                return 0 ;
                        }
                        else if (  strcasecmp(input[0].token.c_str(),"EQU") == 0  && input.size() == 2 ) {
                            if (  input.at(1).type == 6 || input.at(1).type == 5 || strcasecmp(input[1].token.c_str(),"*") == 0 ){
                                if ( input.at(1).type == 6)
                                    space = stringToInt( input.at(1).token) ;
                                return 8 ;
                            }

                            else
                                return 0 ;
                        }
                        else if (  strcasecmp(input[0].token.c_str(),"END") == 0  && input.size() == 2 ) {
                            space = 0 ;
                          //  if (  input.at(1).type == 6)
                                return 9 ;
                        //    else
                           //     return 0 ;
                        }
                        else if (  strcasecmp(input[0].token.c_str(),"END") == 0  && input.size() == 1 ) {
                            space = 0 ;
                            return 9 ;
                        }
                        else if (  strcasecmp(input[0].token.c_str(),"BYTE") == 0   && input.size() == 2 ) {
                            space = 3 ;
                            if (  input.at(1).type == 6)
                                return 5 ;
                            else
                                return 0 ;
                        }
                        else if (  strcasecmp(input[0].token.c_str(),"BYTE") == 0   && input.size() == 5 ) {
                            if (  strcasecmp(input[1].token.c_str(),"C") == 0  &&   strcasecmp(input[2].token.c_str(),"\'") == 0 &&  input.at(3).type == 7 &&  strcasecmp(input[4].token.c_str(),"\'") == 0) {
                                string temp = stringToCode( input.at(4).token) ;
                                return 5 ;
                            }
                            else if ( strcasecmp(input[1].token.c_str(),"X") == 0  &&   strcasecmp(input[2].token.c_str(),"\'") == 0 &&  input.at(3).type == 6 &&  strcasecmp(input[4].token.c_str(),"\'") == 0 ) {
                                space = input.at(4).token.size() / 2 ;
                                return 5 ;
                            }
                            else
                                return 0 ;
                        }
                        else if (  strcasecmp(input[0].token.c_str(),"WORD") == 0   && input.size() == 5 ) {
                            space = 3 ;
                            if (  strcasecmp(input[1].token.c_str(),"C") == 0  &&   strcasecmp(input[2].token.c_str(),"\'") == 0 &&  input.at(3).type == 7 &&  strcasecmp(input[4].token.c_str(),"\'") == 0)
                                return 5 ;
                            else if ( strcasecmp(input[1].token.c_str(),"X") == 0  &&   strcasecmp(input[2].token.c_str(),"\'") == 0 &&  input.at(3).type == 6 &&  strcasecmp(input[4].token.c_str(),"\'") == 0 )
                                return 5 ;
                            else
                                return 0 ;
                        }
                        else if (  strcasecmp(input[0].token.c_str(),"LTORG") == 0  && input.size() == 1 ) {
                            space = 0 ;
                            return 9 ;
                        }
                        else if (  strcasecmp(input[0].token.c_str(),"BASE") == 0  && input.size() == 2 ) {
                            space = 0 ;
                            if (  input.at(1).type == 6 || input.at(1).type == 5 )
                                return 9 ;
                            else
                                return 0 ;
                        }
                        else if (  strcasecmp(input[0].token.c_str(),"RESB") == 0  && input.size() == 2 ) {
                            if (  input.at(1).type == 6  ) {
                                space = stringToInt( input.at(2).token) ;
                                return 5 ;
                            }
                            else
                                return 0 ;
                        }
                        else if (  strcasecmp(input[0].token.c_str(),"RESW") == 0  && input.size() == 2 ) {
                            if (  input.at(1).type == 6  ) {
                                space = stringToInt( input.at(2).token) *3;
                                return 5 ;
                            }
                            else
                                return 0 ;
                        }
                        else {
                            return 0 ;
                        }
                    }
                    else if (  input.at(0).type == 4 ) { // 為operand
                         if (  strcasecmp(input[0].token.c_str(),"+") == 0 && input.size() != 1 ) {
                            space = 4 ;
                             if ( input.size() == 2 ) {
                                if (input[1].type == 1 )
                                    return 4 ;
                                else
                                    return 0 ;
                             }
                             if ( input.size() == 3 ) {
                                if (input[1].type == 1  && (input[2].type == 5|| input[2].type == 6|| input[2].type == 3) )
                                    return 4 ;
                                else
                                    return 0 ;
                             }
                             if ( input.size() == 4 ) {
                                if ( input[1].type == 1  &&( strcasecmp(input[2].token.c_str(),"#") == 0 || strcasecmp(input[2].token.c_str(),"@") == 0 )  && (input[3].type == 5|| input[3].type == 6 || input[3].type == 3) )
                                    return 4 ;
                                else
                                    return 0 ;
                             }
                            else if ( input.size() == 5 ) {
                                if ( input[1].type == 1  &&(input[2].type == 5|| input[2].type == 6 || input[2].type == 3)  && (input[4].type == 5|| input[4].type == 6|| input[4].type == 3) && strcasecmp(input[3].token.c_str(),",") == 0 )
                                    return 4 ;
                                else
                                    return 0 ;
                             }
                        }
                        else if (  strcasecmp(input[0].token.c_str(),".") == 0 ) {
                            space = 0 ;
                            return 6 ;
                        }
                    }

                    return 0 ;
        }


        string chechReg( string input ) {
            if ( strcasecmp(input.c_str(),"A") == 0 )
                return "0" ;
            else if ( strcasecmp(input.c_str(),"F") == 0 )
                return "6" ;
            else if ( strcasecmp(input.c_str(),"T") == 0 )
                return "5" ;
            else if ( strcasecmp(input.c_str(),"S") == 0 )
                return "4" ;
            else if ( strcasecmp(input.c_str(),"B") == 0 )
                return "3" ;
            else if ( strcasecmp(input.c_str(),"X") == 0 )
                return "1" ;
            else if ( strcasecmp(input.c_str(),"L") == 0 )
                return "2" ;
            else if ( strcasecmp(input.c_str(),"PC") == 0 )
                return "8" ;
            else if ( strcasecmp(input.c_str(),"SW") == 0 )
                return "9" ;

                return "0" ;
        }

        string findOP2 ( string input ) {
             if ( strcasecmp(input.c_str(), table1.at(1).c_str()) == 0 ) {
                 return "000110" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(2).c_str()) == 0 ) {
                 return "010110" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(3).c_str()) == 0 ) {
                 return "100100" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(4).c_str()) == 0 ) {
                 return "010000" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(5).c_str()) == 0 ) {
                 return "101101" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(6).c_str()) == 0 ) {
                 return "001010" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(7).c_str()) == 0 ) {
                 return "100010" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(8).c_str()) == 0 ) {
                 return "101000" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(9).c_str()) == 0 ) {
                 return "001001" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(10).c_str()) == 0 ) {
                 return "011001" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(11).c_str()) == 0 ) {
                 return "100111" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(12).c_str()) == 0 ) {
                 return "110001" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(13).c_str()) == 0 ) {
                 return "110000" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(14).c_str()) == 0 ) {
                 return "111101" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(15).c_str()) == 0 ) {
                 return "001111" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(16).c_str()) == 0 ) {
                 return "001100" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(17).c_str()) == 0 ) {
                 return "001101" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(18).c_str()) == 0 ) {
                 return "001110" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(19).c_str()) == 0 ) {
                 return "010010" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(20).c_str()) == 0 ) {
                 return "000000" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(21).c_str()) == 0 ) {
                 return "011010" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(22).c_str()) == 0 ) {
                 return "010100" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(23).c_str()) == 0 ) {
                 return "011100" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(24).c_str()) == 0 ) {
                 return "000010" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(25).c_str()) == 0 ) {
                 return "011011" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(26).c_str()) == 0 ) {
                 return "011101" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(27).c_str()) == 0 ) {
                 return "000001" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(28).c_str()) == 0 ) {
                 return "110100" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(29).c_str()) == 0 ) {
                 return "001000" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(30).c_str()) == 0 ) {
                 return "011000" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(31).c_str()) == 0 ) {
                 return "100110" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(32).c_str()) == 0 ) {
                 return "110010" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(33).c_str()) == 0 ) {
                 return "010001" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(34).c_str()) == 0 ) {
                 return "110110" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(35).c_str()) == 0 ) {
                 return "101011" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(36).c_str()) == 0 ) {
                 return "010011" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(37).c_str()) == 0 ) {
                 return "101001" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(38).c_str()) == 0 ) {
                 return "101010" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(39).c_str()) == 0 ) {
                 return "111100" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(40).c_str()) == 0 ) {
                 return "111011" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(41).c_str()) == 0 ) {
                 return "000011" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(42).c_str()) == 0 ) {
                 return "011110" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(43).c_str()) == 0 ) {
                 return "010101" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(44).c_str()) == 0 ) {
                 return "100000" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(45).c_str()) == 0 ) {
                 return "110101" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(46).c_str()) == 0 ) {
                 return "000101" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(47).c_str()) == 0 ) {
                 return "011111" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(48).c_str()) == 0 ) {
                 return "111010" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(49).c_str()) == 0 ) {
                 return "100001" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(50).c_str()) == 0 ) {
                 return "000100" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(51).c_str()) == 0 ) {
                 return "000111" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(52).c_str()) == 0 ) {
                 return "010111" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(53).c_str()) == 0 ) {
                 return "100101" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(54).c_str()) == 0 ) {
                 return "101100" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(55).c_str()) == 0 ) {
                 return "111000" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(56).c_str()) == 0 ) {
                 return "111110" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(57).c_str()) == 0 ) {
                 return "001011" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(58).c_str()) == 0 ) {
                 return "101110" ;
             }
             else if ( strcasecmp(input.c_str(), table1.at(59).c_str()) == 0 ) {
                 return "110111" ;
             }

             return "99999";
        }

        string checkNI ( vector < Token > input ) {
            //string result = "" ;
            for ( int i = 0 ; i < input.size() ; i ++) {
                if ( strcasecmp(input[i].token.c_str(),"@") == 0 )
                    return "10" ;
                if ( strcasecmp(input[i].token.c_str(),"#") == 0 )
                    return "01" ;

            }
            return "11" ;
        }

        string checkX ( vector < Token > input ) {
            //string result = "" ;
            for ( int i = 0 ; i < input.size() ; i ++) {
                for ( int j = 0 ; j < table3.size() ; j ++ ) {
                    if ( input.at(i).token == table3.at(j))
                        return "1" ;
                }
            }
            return "0" ;
        }

        string hexToBinary( string input ) {
            string result = "" ;
            for ( int i = 0 ; i < input.size() ; i +=4 ) {
                if ( input[i] == '0' &&input[i+1] == '0' && input[i+2]== '0' && input[i+3]== '1') {
                    result = result + "1" ;
                } // 1
                else if ( input[i] == '0' &&input[i+1] == '0' && input[i+2]== '1' && input[i+3]== '0') {
                    result = result + "2" ;
                } // 2
                else if ( input[i] == '0' &&input[i+1] == '0' && input[i+2]== '1' && input[i+3]== '1') {
                    result = result + "3" ;
                } // 3
                else if ( input[i] == '0' &&input[i+1] == '1' && input[i+2]== '0' && input[i+3]== '0') {
                    result = result + "4" ;
                } // 4
                else if ( input[i] == '0' &&input[i+1] == '1' && input[i+2]== '0' && input[i+3]== '1') {
                    result = result + "5" ;
                } // 5
                else if ( input[i] == '0' &&input[i+1] == '1' && input[i+2]== '1' && input[i+3]== '0') {
                    result = result + "6" ;
                } // 6
                else if ( input[i] == '0' &&input[i+1] == '1' && input[i+2]== '1' && input[i+3]== '1') {
                    result = result + "7" ;
                } // 7
                else if ( input[i] == '1' &&input[i+1] == '0' && input[i+2]== '0' && input[i+3]== '0') {
                    result = result + "8" ;
                } // 8
                else if ( input[i] == '1' &&input[i+1] == '0' && input[i+2]== '0' && input[i+3]== '1') {
                    result = result + "9" ;
                } // 9
                else if ( input[i] == '1' &&input[i+1] == '0' && input[i+2]== '1' && input[i+3]== '0') {
                    result = result + "A" ;
                } // A
                else if ( input[i] == '1' &&input[i+1] == '0' && input[i+2]== '1' && input[i+3]== '1') {
                    result = result + "B" ;
                } // B
                else if ( input[i] == '1' &&input[i+1] == '1' && input[i+2]== '0' && input[i+3]== '0') {
                    result = result + "C" ;
                } // C
                else if ( input[i] == '1' &&input[i+1] == '1' && input[i+2]== '0' && input[i+3]== '1') {
                    result = result + "D" ;
                } // D
                else if ( input[i] == '1' &&input[i+1] == '1' && input[i+2]== '1' && input[i+3]== '0') {
                    result = result + "E" ;
                } // E
                else if ( input[i] == '1' &&input[i+1] == '1' && input[i+2]== '1' && input[i+3]== '1') {
                    result = result + "F" ;
                } // F
                else if ( input[i] == '0' &&input[i+1] == '0' && input[i+2]== '0' && input[i+3]== '0') {
                    result = result + "0" ;
                } // 0

     //           cout << i << endl ;

            }

            return result ;
        }

        int findLocation ( string input ) {
            for( int i = 0 ; i < table5.size() ; i++ ) {
                if ( strcasecmp(input.c_str(),table5.at(i).token.c_str() ) == 0 ) {
                    return table5.at(i).place ;
                }
            }

            return 0 ;
         }

        int findLocation2 ( string input, int pc ) {
            int place = 0 ;
            for( int i = 0 ; i < table5.size() ; i++ ) {
                if ( strcasecmp(input.c_str(),table5.at(i).token.c_str() ) == 0 ) {
                    place =  table5.at(i).place ;
                    break ;
                }
            }

            while ( locationTemp.at(pc) == -1 ||  locationTemp.at(pc) == -2) {
                pc++ ;
            }
          // cout << place <<"   place  " << locationTemp.at(pc)  << endl ;
            place = place - locationTemp.at(pc) ;
       //       cout << "oplace  " << dec<< place << endl ;
            if ( place < 0) {
                place = 4096 + place ;
            }
          //              cout << "input  " << input << endl ;
          //  cout << "place  " << place << endl ;
      //      cout << "pc  " << locationTemp.at(pc) << endl ;
            return place ;
         } //find disp
        string translate( vector < Token > input, int line) {
            string result = "";
            for ( int i = 0 ; i < input.size() ; i ++) {
                if ( strcasecmp(input[i].token.c_str(),"+") == 0 ) {
                    if ( strcasecmp(input[i+2].token.c_str(),"#") == 0 ) {
                        result = result + findOP2( input.at(i+1).token ) ;
                        result = result + checkNI( input ) ;
                        result = result + checkX( input ) ;
                        result = result + "0010000" ;
                        result = hexToBinary( result ) ;
                        cout << "\t\t" <<result ;
                        cout << setw(4) << setfill('0') << setiosflags( ios::uppercase) <<  stringToInt(input.at(i+3).token) ;
                        newFile << "\t\t" <<result ;
                        newFile << setw(4) << setfill('0') << setiosflags( ios::uppercase) <<  stringToInt(input.at(i+3).token) ;

                    }
                    else {
                        result = result + findOP2( input.at(i+1).token ) ;
                        result = result + checkNI( input ) ;
                        result = result + checkX( input ) ;
                        result = result + "0010000" ;
                    //cout<< result << "     " ;
                        result = hexToBinary( result ) ;
                        cout << "\t\t" <<result ;
                        cout << setw(4) << setfill('0') << setiosflags( ios::uppercase) << findLocation( input.at(i+2).token) ;
                         newFile << "\t\t" <<result ;
                        newFile << setw(4) << setfill('0') << setiosflags( ios::uppercase) << findLocation( input.at(i+2).token) ;
                    }

                    return result ;
                }
                else if (strcasecmp(input[i].token.c_str(),"STX") == 0  || strcasecmp(input[i].token.c_str(),"LDT") == 0 ) {
                        result = result + findOP2( input.at(i).token ) ;
                        result = result + checkNI( input ) ;
                        result = result + checkX( input ) ;
                        result = result + "100000000000000" ;
                        result = hexToBinary( result ) ;
                        cout << "\t\t" <<result ;
                        newFile << "\t\t" <<result ;
                }
                else if (strcasecmp(input[i].token.c_str(),"STCH") == 0  || strcasecmp(input[i].token.c_str(),"LDCH") == 0 ) {
                        result = result + findOP2( input.at(i).token ) ;
                        result = result + checkNI( input ) ;
                        result = result + checkX( input ) ;
                        result = result + "100000000000011" ;
                        result = hexToBinary( result ) ;
                        cout << "\t\t" <<result ;
                        newFile << "\t\t" <<result ;
                }
                else if (strcasecmp(input[i].token.c_str(),"BYTE") == 0  ) {
                    if ( strcasecmp(input[i+1].token.c_str(),"X") == 0  ) {
                        cout << "\t\t" << input[i+3].token ;
                         newFile << "\t\t" << input[i+3].token ;
                    }
                    else if ( strcasecmp(input[i+1].token.c_str(),"C") == 0  ) {
                        cout << "\t\t" << stringToCode(input[i+3].token );
                         newFile << "\t\t" << stringToCode(input[i+3].token );
                    }
                }
                else if ( strcasecmp(input[i].token.c_str(),"ADDR") == 0 || strcasecmp(input[i].token.c_str(),"COMPR") == 0 || strcasecmp(input[i].token.c_str(),"DIVR") == 0 || strcasecmp(input[i].token.c_str(),"MULR") == 0||strcasecmp(input[i].token.c_str(),"RMO")==0 || strcasecmp(input[i].token.c_str(),"SHIFTL") == 0||strcasecmp(input[i].token.c_str(),"SHIFTR" ) == 0 ||strcasecmp(input[i].token.c_str(),"SUBR" )==0 ) {
                        result = result + checkCode( input.at(i)) ;
                        result = result + chechReg( input.at(i+1).token) ;
                        result = result + chechReg( input.at(i+3).token) ;
                        cout << "\t\t" <<result ;
                        newFile << "\t\t" <<result ;
                        return result ;
                }
                else if (strcasecmp(input[i].token.c_str(),"CLEAR") == 0 || strcasecmp(input[i].token.c_str(),"SVC") == 0 || strcasecmp(input[i].token.c_str(),"TIXR") == 0 ) {
                        result = result + checkCode( input.at(i)) ;
                        result = result + chechReg( input.at(i+1).token) ;
                        result = result +"0" ;
                        cout << "\t\t" <<result ;
                        newFile << "\t\t" <<result ;
                        return "";
                }
                else if ( strcasecmp(input[i].token.c_str(),".") == 0) {
                    return "";
                }
                else if ( strcasecmp(input[i].token.c_str(),"RSUB") == 0) {
                    cout << "\t\t4F0000"  ;
                    newFile << "\t\t4F0000"  ;
                    return "";
                }
                else if ( strcasecmp(input[i].token.c_str(),"START") == 0 || strcasecmp(input[i].token.c_str(),"RESB") == 0 || strcasecmp(input[i].token.c_str(),"RESW") == 0 || strcasecmp(input[i].token.c_str(),"END") == 0 || strcasecmp(input[i].token.c_str(),"BASE") == 0||strcasecmp(input[i].token.c_str(),"EQU") == 0)
                    return "" ;
                else if (strcasecmp(input[i].token.c_str(),"FIX") == 0 || strcasecmp(input[i].token.c_str(),"FLOAT") == 0 || strcasecmp(input[i].token.c_str(),"HIO") == 0 || strcasecmp(input[i].token.c_str(),"NORM") == 0 || strcasecmp(input[i].token.c_str(),"SIO") == 0||strcasecmp(input[i].token.c_str(),"TIO") == 0) {
                    cout << "\t\t" << checkCode( input.at(i)) ;
                    newFile << "\t\t" << checkCode( input.at(i)) ;
                    return "" ;
                }
                else if ( input[i].type == 1 ) {
                    if (strcasecmp(input[i+1].token.c_str(),"@") == 0 ) {
                        result = result + findOP2( input.at(i).token ) ;
                        result = result + checkNI( input ) ;
                        result = result + checkX( input ) ;
                        result = result + "010" ;
                        result = hexToBinary( result ) ;
                        cout << "\t\t" <<result << setw(3) << setfill('0') << setiosflags( ios::uppercase) << hex << setw(3) << findLocation2( input.at(i+2).token , line+1) ;
                         newFile << "\t\t" <<result << setw(3) << setfill('0') << setiosflags( ios::uppercase) << hex << setw(3) << findLocation2( input.at(i+2).token , line+1) ;
                    }
                    else if (strcasecmp(input[i+1].token.c_str(),"#") == 0 ) {
                        result = result + findOP2( input.at(i).token ) ;
                        result = result + checkNI( input ) ;
                        result = result + checkX( input ) ;
                        if ( input.at(i+2).type == 5 )
                            result = result + "010" ;
                        else
                            result = result + "000" ;
                        result = hexToBinary( result ) ;
                        if ( input.at(i+2).type == 5 ) {
                            cout << "\t\t" <<result << setw(3) << setfill('0') << setiosflags( ios::uppercase) << hex << setw(3) << findLocation2( input.at(i+2).token , line+1) ;
                            newFile << "\t\t" <<result << setw(3) << setfill('0') << setiosflags( ios::uppercase) << hex << setw(3) << findLocation2( input.at(i+2).token , line+1) ;
                        }

                        else {
                            cout << "\t\t" <<result << setw(3) << setfill('0') << setiosflags( ios::uppercase) << hex << setw(3) << stringToInt( input.at(i+2).token ) ;
                            newFile << "\t\t" <<result << setw(3) << setfill('0') << setiosflags( ios::uppercase) << hex << setw(3) << stringToInt( input.at(i+2).token ) ;
                        }
                    }
                    else {
                        result = result + findOP2( input.at(i).token ) ;
                        result = result + checkNI( input ) ;
                        result = result + checkX( input ) ;
                         if ( input.at(i+1).type == 5)
                            result = result + "010" ;
                        else
                            result = result + "000" ;
                        result = hexToBinary( result ) ;
                        cout << "\t\t" <<result ;
                        newFile << "\t\t" <<result ;
                        if ( input.at(i+1).type == 5) {
                             cout << setw(3) << setfill('0') << setiosflags( ios::uppercase) << hex << setw(3) << findLocation2( input.at(i+1).token , line+1) ;
                              newFile << setw(3) << setfill('0') << setiosflags( ios::uppercase) << hex << setw(3) << findLocation2( input.at(i+1).token , line+1) ;
                        }

                        else {
                            cout << setw(3) << setfill('0') << setiosflags( ios::uppercase) << hex << setw(3) << stringToInt( input.at(i+1).token) ;
                            newFile << setw(3) << setfill('0') << setiosflags( ios::uppercase) << hex << setw(3) << stringToInt( input.at(i+1).token) ;
                        }

                    }
                    return result ;
                }
                }

            return "0" ;
        }
        void print2() {
            cout <<  "Line  Location  Source code                              Object code" << endl ;
            cout << "----  -------- -------------------------                 -----------" << endl ;
            int line = 0 ;

            int location = findStart() ;
            int startLocation = location ;
            string codeTemp ="";
            int needSpace ;
            for ( int i = 0 ; i < tokenTemp.size() ; i++ ) {

                //int j = 0 ;
                //if ( tokenTemp.at(i).size() > 0) {
                 cout << "NMLKJDLS" << endl ;
                int  theFormat= checkLine ( tokenTemp.at(i) , needSpace) ;
               //cout << theFormat  << tokenTemp.at(i).at(0).token<< endl ;
                if ( theFormat ) {

                        if ( theFormat == 8 ) {
                            location = needSpace ;
                             //cout << "lo:         " << hex << location << endl ;
                              if ( tokenTemp.at(i).at(0).type == 5 )
                                setLocation( tokenTemp.at(i).at(0), location) ;
                             locationTemp.push_back( location) ;
                             location += 3 ;
                        }
                        else {
                            //cout << needSpace << "      lo:         " << hex << location << endl ;
                            if ( tokenTemp.at(i).at(0).type == 5 && theFormat != 9 && theFormat != 6) {
                                setLocation( tokenTemp.at(i).at(0), location) ;
                                locationTemp.push_back( location) ;
                            }
                            else if ( theFormat != 9 && theFormat != 6)
                                locationTemp.push_back( location) ;
                            else
                                locationTemp.push_back( -1) ;
                            location = needSpace + location ;
                        }


                }
                else {
                        tokenTemp.at(i).at(0).token  = "error" ; // type 8
                        locationTemp.push_back( -2) ;
                }

            } // PASS1


            location = startLocation ;

            for ( int i = 0 ; i < tokenTemp.size() ; i++ ) {
                if ( locationTemp.at(i) == -2 ) {
                    cout << "syntax error!!" ;
                }
            else {
                line += 5 ;
                 for ( int j = 0 ; j < tokenTemp.at(i).size() ; j++ ) {
                        if ( tokenTemp.at(i).at(j).type == 5 ) { // 為symbol
                           if ( locationTemp.at(i) == -1 )
                                cout << setw(4)  << setfill(' ')<< dec << line << "\t"  << "\t" << tokenTemp.at(i).at(j).token << "\t" << tokenTemp.at(i).at(j+1).token   ;
                            else
                                cout << setw(4)  << setfill(' ')<< dec << line << "\t" << setw(4) << setfill('0') << setiosflags( ios::uppercase) << hex<< locationTemp.at(i) << "\t" << tokenTemp.at(i).at(j).token << "\t" << tokenTemp.at(i).at(j+1).token   ;
                            j+= 1 ;
                            if ( j < tokenTemp.at(i).size()) {
                                if ( tokenTemp.at(i).at(j).type== 4 && (j+1) < tokenTemp.at(i).size()) {//檢查下個是否為符號 +add
                                    cout << tokenTemp.at(i).at(j+1).token << "\t";
                                    j++ ;
                                }
                                else {
                                    cout << "\t" ;

                                }

                                 j++ ;
                            }

                            while( j < tokenTemp.at(i).size() ) {
                                cout << tokenTemp.at(i).at(j).token ;
                                j++ ;
                            }
                        }
                        else  if ( tokenTemp.at(i).at(j).type == 1 || tokenTemp.at(i).at(j).type == 2   ) { //首字即為指令
                             if ( strcasecmp(tokenTemp.at(i).at(j).token.c_str(), "LTORG") == 0 || strcasecmp(tokenTemp.at(i).at(j).token.c_str(), "END") == 0)
                                cout << setw(4)  << setfill(' ')<< dec << line << "\t"  << "\t\t" << tokenTemp.at(i).at(j).token  ;
                           else if ( locationTemp.at(i) == -1 )
                                cout << setw(4)  << setfill(' ')<< dec << line << "\t"  << "\t\t" << tokenTemp.at(i).at(j).token  ;
                            else
                                cout << setw(4)  << setfill(' ')<< dec << line << "\t" << setw(4) << setfill('0') << setiosflags( ios::uppercase) << hex<< locationTemp.at(i) << "\t\t" << tokenTemp.at(i).at(j).token  ;
                            if ( (j+1) >= tokenTemp.at(i).size()) ;
                            else {
                                    cout << "\t" << tokenTemp.at(i).at(j+1).token   ;
                                    j+= 1 ;
                                    if ( j <  tokenTemp.at(i).size())
                                        j++ ;
                                    while( j < tokenTemp.at(i).size() ) {
                                        cout << tokenTemp.at(i).at(j).token ;
                                        j++ ;
                                    }
                            }


                        }
                        else if (  tokenTemp.at(i).at(j).type ==4 ) {
                            if ( tokenTemp.at(i).size() !=1 ) {
                                if ( tokenTemp.at(i).at(j+1).type ==1 ) {
                                    cout << setw(4)  << setfill(' ')<< dec << line << "\t" << setw(4) << setfill('0') << setiosflags( ios::uppercase) << hex<<  locationTemp.at(i)  << "\t\t" << tokenTemp.at(i).at(j).token  << tokenTemp.at(i).at(j+1).token  << "\t" ;
                                    j+= 1 ;
                                    if ( j <  tokenTemp.at(i).size())
                                        j++ ;
                                    while( j < tokenTemp.at(i).size() ) {
                                        cout << tokenTemp.at(i).at(j).token ;
                                        j++ ;
                                    }
                                }
                                else  if ( tokenTemp.at(i).at(j+1).type == 7 ) {
                                     cout << setw(4)  << setfill(' ')<< dec << line << "\t"  << "\t\t" << tokenTemp.at(i).at(j).token << tokenTemp.at(i).at(j+1).token ;
                                    j++;
                                }
                            }
                            else if ( tokenTemp.at(i).size() ==1 && strcasecmp(tokenTemp.at(i).at(j).token.c_str(), ".") == 0 ) {
                                cout << setw(4)  << setfill(' ')<< dec << line << "\t"  << "\t\t" << tokenTemp.at(i).at(j).token ;
                            }
                        }
                        else {
                            //cout <<  tokenTemp.at(i).at(0).token << endl ;
                            cout << "Erro!!" ;
                        }

                 }

                 translate( tokenTemp.at(i), i ) ;
            }
                 cout << endl ;

            } // PASS2*/
        }

        void write2() {
            cout << "-----------------------------------------------------" << endl ;
            newFile.open("My"+ fileName + "_output.txt", ios::out);
            newFile <<  "Line  Location  Source code                              Object code" << endl ;
            newFile << "----  -------- -------------------------                 -----------" << endl ;
            int line = 0 ;
            int location = findStart() ;
            int startLocation = location ;
            string codeTemp ;
            int needSpace ;
            for ( int i = 0 ; i < tokenTemp.size() ; i++ ) {
                //int j = 0 ;
                //if ( tokenTemp.at(i).size() > 0) {
                int  theFormat= checkLine ( tokenTemp.at(i) , needSpace) ;
             //  newFile << theFormat  << tokenTemp.at(i).at(0).token<< endl ;
                if ( theFormat ) {
                        if ( theFormat == 8 ) {
                            location = needSpace ;
                             //cout << "lo:         " << hex << location << endl ;
                              if ( tokenTemp.at(i).at(0).type == 5 )
                                setLocation( tokenTemp.at(i).at(0), location) ;
                             locationTemp.push_back( location) ;
                             location += 3 ;
                        }
                        else {
                            //cout << needSpace << "      lo:         " << hex << location << endl ;
                            if ( tokenTemp.at(i).at(0).type == 5 && theFormat != 9 && theFormat != 6) {
                                setLocation( tokenTemp.at(i).at(0), location) ;
                                locationTemp.push_back( location) ;
                            }
                            else if ( theFormat != 9 && theFormat != 6)
                                locationTemp.push_back( location) ;
                            else
                                locationTemp.push_back( -1) ;
                            location = needSpace + location ;
                        }


                }
                else {
                        tokenTemp.at(i).at(0).token  = "error" ; // type 8
                        locationTemp.push_back( -2) ;
                }

            } // PASS1


            location = startLocation ;

            for ( int i = 0 ; i < tokenTemp.size() ; i++ ) {
                if ( locationTemp.at(i) == -2 ) {
                    newFile << "syntax error!!" ;
                }
            else {
                line += 5 ;
                 for ( int j = 0 ; j < tokenTemp.at(i).size() ; j++ ) {
                        if ( tokenTemp.at(i).at(j).type == 5 ) { // 為symbol
                           if ( locationTemp.at(i) == -1 )
                                newFile << setw(4)  << setfill(' ')<< dec << line << "\t"  << "\t" << tokenTemp.at(i).at(j).token << "\t" << tokenTemp.at(i).at(j+1).token   ;
                            else
                                newFile << setw(4)  << setfill(' ')<< dec << line << "\t" << setw(4) << setfill('0') << setiosflags( ios::uppercase) << hex<< locationTemp.at(i) << "\t" << tokenTemp.at(i).at(j).token << "\t" << tokenTemp.at(i).at(j+1).token   ;
                            j+= 1 ;
                            if ( j < tokenTemp.at(i).size()) {
                                if ( tokenTemp.at(i).at(j).type== 4 && (j+1) < tokenTemp.at(i).size()) {//檢查下個是否為符號 +add
                                    newFile << tokenTemp.at(i).at(j+1).token << "\t";
                                    j++ ;
                                }
                                else {
                                    newFile << "\t" ;

                                }

                                 j++ ;
                            }

                            while( j < tokenTemp.at(i).size() ) {
                                newFile << tokenTemp.at(i).at(j).token ;
                                j++ ;
                            }
                        }
                        else  if ( tokenTemp.at(i).at(j).type == 1 || tokenTemp.at(i).at(j).type == 2   ) { //首字即為指令
                             if ( strcasecmp(tokenTemp.at(i).at(j).token.c_str(), "LTORG") == 0 || strcasecmp(tokenTemp.at(i).at(j).token.c_str(), "END") == 0)
                                newFile << setw(4)  << setfill(' ')<< dec << line << "\t"  << "\t\t" << tokenTemp.at(i).at(j).token  ;
                           else if ( locationTemp.at(i) == -1 )
                                newFile << setw(4)  << setfill(' ')<< dec << line << "\t"  << "\t\t" << tokenTemp.at(i).at(j).token  ;
                            else
                                newFile << setw(4)  << setfill(' ')<< dec << line << "\t" << setw(4) << setfill('0') << setiosflags( ios::uppercase) << hex<< locationTemp.at(i) << "\t\t" << tokenTemp.at(i).at(j).token  ;
                            if ( (j+1) >= tokenTemp.at(i).size()) ;
                            else {
                                    newFile << "\t" << tokenTemp.at(i).at(j+1).token   ;
                                    j+= 1 ;
                                    if ( j <  tokenTemp.at(i).size())
                                        j++ ;
                                    while( j < tokenTemp.at(i).size() ) {
                                        newFile << tokenTemp.at(i).at(j).token ;
                                        j++ ;
                                    }
                            }


                        }
                        else if (  tokenTemp.at(i).at(j).type ==4 ) {
                            if ( tokenTemp.at(i).size() !=1 ) {
                                if ( tokenTemp.at(i).at(j+1).type ==1 ) {
                                    newFile << setw(4)  << setfill(' ')<< dec << line << "\t" << setw(4) << setfill('0') << setiosflags( ios::uppercase) << hex<<  locationTemp.at(i)  << "\t\t" << tokenTemp.at(i).at(j).token  << tokenTemp.at(i).at(j+1).token  << "\t" ;
                                    j+= 1 ;
                                    if ( j <  tokenTemp.at(i).size())
                                        j++ ;
                                    while( j < tokenTemp.at(i).size() ) {
                                        newFile << tokenTemp.at(i).at(j).token ;
                                        j++ ;
                                    }
                                }
                                else  if ( tokenTemp.at(i).at(j+1).type == 7 ) {
                                     newFile << setw(4)  << setfill(' ')<< dec << line << "\t"  << "\t\t" << tokenTemp.at(i).at(j).token << tokenTemp.at(i).at(j+1).token ;
                                    j++;
                                }
                            }
                            else if ( tokenTemp.at(i).size() ==1 && strcasecmp(tokenTemp.at(i).at(j).token.c_str(), ".") == 0 ) {
                                newFile << setw(4)  << setfill(' ')<< dec << line << "\t"  << "\t\t" << tokenTemp.at(i).at(j).token ;
                            }
                        }
                        else {
                            //cout <<  tokenTemp.at(i).at(0).token << endl ;
                            newFile << "Erro!!" ;
                        }

                 }

                 translate( tokenTemp.at(i), i ) ;
            }
                 newFile << endl ;

            } // PASS2*/

            newFile.close() ;
        }
};




int main(){
    SIC a ;
    bool check = false ;
   	int command = 0 ; // user command
  	do {

  		cout << endl << "**** Assembler *****" ;
  		cout << endl << "* 0. QUIT                  *" ;
  		cout << endl << "* 1. SIC      *" ;
  		cout << endl << "* 2. SIC\XE      *" ;
  		cout << endl << "*************************************" ;
  		cout << endl << "Input a command( 0, 1, 2 ) : " ;
  		cin >> command ; // get the command
  		Token k ;
  		switch(command){
			case 0 : break ; //跳出，結束

  			case 1 : //
  			    a.reset() ;
                a.readFixedTable(  ) ;
                a.getToken();
                a.print() ;
                a.write() ;
                //a.checkCode(k) ;
  				break ;

            case 2 :
                a.reset() ;
                a.readFixedTable(  ) ;
                a.getToken();
                a.print2() ;
                a.write2() ;
                break ;
            case 3 :

                break ;
  			default :
				cout << endl << "command does not exist !!" << endl ;
  				break ;

		} // end switch

  	} while ( command != 0 ) ;

  	/*string text1 = "add" ;
                string text2 = "ADD" ;
                if ( strcasecmp(text1.c_str(), text2.c_str()) == 0 ) {
                    cout << "OK" << endl ;
                }*/
	system( "pause") ; // pause the display
  	return 0 ;
}
