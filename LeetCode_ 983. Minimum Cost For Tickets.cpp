class Solution {
public:
    int mincostTickets(vector<int>& days, vector<int>& costs) {
      int dp[366] = {0}, need[366] = {0} ;
      int daysIndex = 0, result = 0 ;
      for ( int i = 0 ; i < days.size() ; i++ ) {
        ++need[days[i]] ;
      } 

      int low =  min( costs[0], min( costs[1], costs[2] ) ) ;
      for ( int i = 1 ; i < 366 ; i++ ) {
        if ( need[i] == 0 )
          dp[i] = dp[i-1] ;
        else 
          dp[i] =  dp[i-1] + low ;
          
        if ( i >= 7 ) {
            dp[i] = min( dp[i], dp[i-7] + costs[1] ) ;
        }
        if ( i >= 30 ) {
            dp[i] = min( dp[i], dp[i-30] + costs[2] ) ;
        }
        
      }


      return dp[365]; 
    }
};