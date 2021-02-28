//tested second commit

#include<bits/stdc++.h>
using namespace std;
#define pb push_back

int binomial_array[10] = {0,3,1,0,0,1,0,1,2,0};
double fraction = 0.5;
int time_frames = 5, link = 4, deadline = 4;
double up_Weight[6];
double down_Weight[6];


//INPUT GENERATION------------------------------------------
void packetGenerator(int deadline_lower, int deadline_upper,vector<vector<vector<int>>> &tou)
{
    int cnt = 0;
    cout<<"deadline_lower: "<<deadline_lower<<endl<<"deadline_upper: "<<deadline_upper<<"\n";

    for(int k = 0 ;k<time_frames;k++)
    {
      for(int i = 1 ;i<link;i++)
      {
        for(int j = deadline_lower;j<deadline_upper;j++)
        {
            tou[k][i][j] = binomial_array[rand()%10];
            //srand(time(NULL));
            cnt += tou[k][i][j];
            cout<<tou[k][i][j]<<" ";
        }

        cout<<"\n";
      }
      cout<<"\n\n";
    }

    cout<<"Total number of packets generated = "<<cnt<<"\n\n";
}




void initialize_Buffer(int deadline_lower, int deadline_upper,vector<vector<vector<int>>> &buffer,vector<vector<vector<int>>> &tou)
{

  //cout<<"Initialized buffer below---------------"<<endl;
    for(int k = 0 ;k<time_frames;k++)
    {
        for(int i = 1 ;i<link;i++)
        {
            for(int j = deadline_lower;j<deadline_upper;j++)
            {
                if(k == 0)
                    buffer[k][i][j] = tou[k][i][j];
                else
                    buffer[k][i][j] = 0;

            //cout<<buffer[k][i][j]<<" ";
            }
          //cout<<"\n";
        }

    //cout<<"\n\n";
    }
}



int up_final_link_chosen(int k, int deadline_lower, int deadline_upper, vector<vector<vector<int>>> &buffer)
{
	
	int earliest_Deadline[link];
	
//	cout<<"earliest deadline----\n"<<endl;
    for(int i = 1; i<link; i++)
    {
        bool f = false;
        for(int j = deadline_lower; j<deadline_upper; j++)
        {
            if(buffer[k][i][j] != 0)
            {
                earliest_Deadline[i] = j;
                f = true;
                break;
            }
        }
        if(f == false)
           earliest_Deadline[i] = -1;

     }
     

     //-------------------------------- non dom links ------------------------------------------
    vector<int>non_Dominated_Links;
    vector<double>v1;
	
    for(int i = 1; i<link; i++)
    {
        v1.push_back(up_Weight[i]);
    }

    //cout<<"\n";

    int pole = 0;
    while(1)
    {
      	double mx = -1;
      // if(pole > 2 )break;
      // pole ++;
        vector<int>v2;
        for(int i = 0; i<v1.size(); i++)
        {
            if(v1[i] > mx)
            {
                mx = v1[i];
            }
        }

        //cout<<"mx = "<<mx<<"\n";

        if(mx <= 0)
            break;

        for(int i = 1; i<link; i++)
        {
            if(up_Weight[i] == mx)
            {
                v2.push_back(i);
            }
        }

        //cout<<"v2 ka data----\n"<<endl;
        /*
        for(int i = 0;i<v2.size();i++)
        cout<<v2[i]<<" ";
        cout<<"\n";
		*/
        int len = v2.size();
        int non_dom = v2[0];
        for(int i = 0; i<len; i++)
        {
            if(earliest_Deadline[v2[i]] < earliest_Deadline[non_dom])
            {
                non_dom = v2[i];
            }
        }




        non_Dominated_Links.push_back(non_dom);
        //cout<<"non dom wala  "<<non_dom<<endl;

        for(int i=1; i<link; i++)
        {	
            if(earliest_Deadline[i] >= earliest_Deadline[non_dom]  || (earliest_Deadline[i] == -1))
            {
                v1[i-1] = -1;
            }
            
        }

		/*
        for(int i=0; i<v1.size(); i++)
        {
            cout<<v1[i]<<" ";
        }
        cout<<"\n";
		*/


        //cout<<"lele"<<endl;

        }

    // }

    cout<<"Non dominated links for Uplinks: ";
    for(int i=0;i<non_Dominated_Links.size();i++)
    {
        cout<<non_Dominated_Links[i]<<" ";
    }
    cout<<endl;


  //--------------------- algo-2------------------------------------------
  int non_dom_size = non_Dominated_Links.size();
  double p_h = 999;
  vector<double>non_dom_prob;
  double r = 1;
  for(int i = 0; i<non_dom_size-1; i++) {
  		p_h = min(1 - ((double)up_Weight[non_Dominated_Links[i+1]]/(double)up_Weight[non_Dominated_Links[i]]) , r);
  		r = r - p_h;
  		non_dom_prob.pb(p_h);
  }
  non_dom_prob.pb(r);
  
  
  
  //cout<<endl;
  
  double random_num = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
  srand (time(NULL));
  random_num = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
  
  //cout<<"random num is --------------  "<<random_num<<endl;
  
  
  double prefix_sum[non_dom_size];
  prefix_sum[0] = non_dom_prob[0];
  for(int i=1; i<non_dom_size; i++) {
  		prefix_sum[i] = prefix_sum[i-1] + non_dom_prob[i];
  }
  
  
  //cout<<"prefix sum array -----"<<endl;
  //for(int i=0; i<non_dom_size; i++) {
  	//		cout<<prefix_sum[i]<<" ";
  //}
	//cout<<endl;
	
	
	int chosen_link = -1;
	for(int i=0; i<non_dom_size; i++) {
  		if(prefix_sum[i] >= random_num) {
  			chosen_link = i+1;
  			break;
  		}
  	}
  	cout<<"Final link chosen for Uplink: "<<chosen_link;
  	cout<<"\n\n";

    return chosen_link;
}






///######################## down_final_link ###############################
int down_final_link_chosen(int k, int deadline_lower, int deadline_upper, vector<vector<vector<int>>> &buffer)
{
	
	int earliest_Deadline[link];
	
//	cout<<"earliest deadline----\n"<<endl;
    for(int i = 1; i<link; i++)
    {
        bool f = false;
        for(int j = deadline_lower; j<deadline_upper; j++)
        {
            if(buffer[k][i][j] != 0)
            {
                earliest_Deadline[i] = j;
                f = true;
                break;
            }
        }
        if(f == false)
           earliest_Deadline[i] = -1;

     }
     

     //-------------------------------- non dom links ------------------------------------------
    vector<int>non_Dominated_Links;
    vector<double>v1;
	
    for(int i = 1; i<link; i++)
    {
        v1.push_back(down_Weight[i]);
    }

    //cout<<"\n";

    int pole = 0;
    while(1)
    {
      	double mx = -1;
      // if(pole > 2 )break;
      // pole ++;
        vector<int>v2;
        for(int i = 0; i<v1.size(); i++)
        {
            if(v1[i] > mx)
            {
                mx = v1[i];
            }
        }

        //cout<<"mx = "<<mx<<"\n";

        if(mx <= 0)
            break;

        for(int i = 1; i<link; i++)
        {
            if(down_Weight[i] == mx)
            {
                v2.push_back(i);
            }
        }


        int len = v2.size();
        int non_dom = v2[0];
        for(int i = 0; i<len; i++)
        {
            if(earliest_Deadline[v2[i]] < earliest_Deadline[non_dom])
            {
                non_dom = v2[i];
            }
        }




        non_Dominated_Links.push_back(non_dom);
        //cout<<"non dom wala  "<<non_dom<<endl;

        for(int i=1; i<link; i++)
        {	
            if(earliest_Deadline[i] >= earliest_Deadline[non_dom]  || (earliest_Deadline[i] == -1))
            {
                v1[i-1] = -1;
            }
            
        }

		/*
        for(int i=0; i<v1.size(); i++)
        {
            cout<<v1[i]<<" ";
        }
        cout<<"\n";
		*/


        //cout<<"lele"<<endl;

        }


    cout<<"Non dominated links for Downlinks: ";
    for(int i=0;i<non_Dominated_Links.size();i++)
    {
        cout<<non_Dominated_Links[i]<<" ";
    }
    //cout<<endl;


  //--------------------- algo-2------------------------------------------
  int non_dom_size = non_Dominated_Links.size();
  double p_h = 999;
  vector<double>non_dom_prob;
  double r = 1;
  for(int i = 0; i<non_dom_size-1; i++) {
  		p_h = min(1 - ((double)down_Weight[non_Dominated_Links[i+1]]/(double)down_Weight[non_Dominated_Links[i]]) , r);
  		r = r - p_h;
  		non_dom_prob.pb(p_h);
  }
  non_dom_prob.pb(r);
  
  
  
  //cout<<endl;
  
  double random_num = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
  srand (time(NULL));
  random_num = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
  
  //cout<<"random num is --------------  "<<random_num<<endl;
  
  
  	double prefix_sum[non_dom_size];
  	prefix_sum[0] = non_dom_prob[0];
  	for(int i=1; i<non_dom_size; i++) {
  			prefix_sum[i] = prefix_sum[i-1] + non_dom_prob[i];
  	}
  
	cout<<endl;

	int chosen_link = -1;
	for(int i=0; i<non_dom_size; i++) {
  		if(prefix_sum[i] >= random_num) {
  			chosen_link = i+1;
  			break;
  		}
  	}
  	cout<<"Final link chosen for Downlink: "<<chosen_link;
  	cout<<"\n\n";

    return chosen_link;
}





// --------------------- updating buffer -------------------------------------------------------
void update_Buffer(int deadline_lower, int deadline_upper, vector<vector<vector<int>>> &up_buffer, vector<vector<vector<int>>> &down_buffer, vector<vector<vector<int>>> &up_tou, vector<vector<vector<int>>> &down_tou)
{	
    int up_I_t[time_frames][link];
    int down_I_t[time_frames][link];
    
    for(int k = 0; k<time_frames-1; k++)
    {
    	cout<<endl;
    	cout<<"################ For time frame = "<<k+1<<" ################ "<<endl<<endl;
    	int up_link_picked = up_final_link_chosen(k, deadline_lower, deadline_upper, up_buffer);
    	
    	int down_link_picked = down_final_link_chosen(k, deadline_lower, deadline_upper, down_buffer);
    	//cout<<"Updated Buffer below---------------"<<endl;
        for(int i = 1; i<link; i++)
        {
            for(int j = deadline_lower; j<deadline_upper; j++)
            {
                up_buffer[k+1][i][j] = up_buffer[k+1][i][j] + up_buffer[k][i][j+1] + up_tou[k+1][i][j];
                
                down_buffer[k+1][i][j] = down_buffer[k+1][i][j] + down_buffer[k][i][j+1] + down_tou[k+1][i][j];
                //cout<<buffer[k+1][i][j]<<" ";
            }
            //cout<<endl;
        }
		
		int up_earliest_Deadline, down_earliest_Deadline;
		int up_i = up_link_picked, down_i = down_link_picked;
		bool f = false;
	    // cout<<"heyy\n\n";

	    for(int j = deadline_lower; j<deadline_upper; j++)
	    {
	        if(up_buffer[k][up_i][j] != 0)
	        {
	            up_earliest_Deadline = j;
	            f = true;
	            break;
	        }
	    }
	    
	    if(f == false)
	       up_earliest_Deadline = -1;

		up_buffer[k+1][up_link_picked][up_earliest_Deadline-1]--;
		
		
		//---------------- for down link finding the earliest deadline
		f = false;

	    for(int j = deadline_lower; j<deadline_upper; j++)
	    {
	        if(down_buffer[k][down_i][j] != 0)
	        {
	            down_earliest_Deadline = j;
	            f = true;
	            break;
	        }
	    }
	    
	    if(f == false)
	       down_earliest_Deadline = -1;

		down_buffer[k+1][down_link_picked][down_earliest_Deadline-1]--;
		
		
		
		
		cout<<endl;
		cout<<"Updated Buffer for Uplinks below ---------------"<<endl;
        for(int i = 1; i<link; i++)
        {
            for(int j = deadline_lower; j<deadline_upper; j++)
            {
                cout<<up_buffer[k+1][i][j]<<" ";
            }
            cout<<endl;
        }
        cout<<endl;
        
        
        
        cout<<"Updated Buffer for Downlinks below---------------"<<endl;
        for(int i = 1; i<link; i++)
        {
            for(int j = deadline_lower; j<deadline_upper; j++)
            {
                cout<<down_buffer[k+1][i][j]<<" ";
            }
            cout<<endl;
        }
        cout<<endl;
		
		//w_(t+1) = w_t + a_t - I_t
		for(int i = 1; i<link; i++)
		{
		    int up_wt = 0, down_wt = 0;
		    for(int j = 1; j<deadline; j++)
		    {
		        up_wt += up_buffer[k][i][j];
		        down_wt += down_buffer[k][i][j];
		    }
		    
		    up_Weight[i] = up_Weight[i] + (up_wt/2.0);
		    down_Weight[i] = down_Weight[i] + (down_wt/2.0);
		    
		    if(i == up_link_picked)
		    	up_Weight[i]--;
		    if(i == down_link_picked)
		    	down_Weight[i]--;
		}
    } 
}


/*----------------------------------------------------MAIN FUNC-----------------------------------------------------*/
int main()
{

    //int time_frames = 3 , link = 4, deadline = 4;

    // links used 0 1 2 3, deadline used 0 1 2 3, no packet on deadline 0, no packets on link 0;

    vector<vector<vector<int>>> up_Tou(time_frames, vector<vector<int>>(link+1, vector<int>(deadline)));
    vector<vector<vector<int>>> down_Tou(time_frames, vector<vector<int>>(link+1, vector<int>(deadline)));


    for(int k = 0 ;k<time_frames;k++)
    {
        for(int i = 1 ;i<link;i++)
        {
            for(int j = 1;j<deadline;j++)
            {
                up_Tou[k][i][j] = 0;
                down_Tou[k][i][j] = 0;
            }
        }
    }


    int deadline_lower = 1, deadline_upper = deadline;
    cout<<"Uplink Packets -->"<<endl;
    packetGenerator(deadline_lower,deadline_upper, up_Tou);
    cout<<"Downlink Packets -->"<<endl;
    packetGenerator(deadline_lower,deadline_upper, down_Tou);

	/*
    up_Tou[0][1][3] = 3;
    up_Tou[0][2][1] = 0;
    up_Tou[0][2][2] = 2;
    up_Tou[0][2][3] = 0;
	up_Tou[0][3][1] = 1;
    up_Tou[0][3][2] = 0;
     
     
      
    down_Tou[0][1][3] = 3;
    down_Tou[0][2][1] = 0;
    down_Tou[0][2][2] = 2;
    down_Tou[0][2][3] = 0;
    down_Tou[0][3][1] = 1;
    down_Tou[0][3][2] = 0;
    */

    //   packetGenerator(time_frames,link,deadline,deadline_lower,deadline_upper,down_Tou);

    vector<vector<vector<int>>> up_Buffer(time_frames, vector<vector<int>>(link+1, vector<int>(deadline)));
    vector<vector<vector<int>>> down_Buffer(time_frames, vector<vector<int>>(link+1, vector<int>(deadline)));


    initialize_Buffer(deadline_lower,deadline_upper,up_Buffer,up_Tou);
    initialize_Buffer(deadline_lower,deadline_upper,down_Buffer,down_Tou);
	

    // -------------------------- Initialized up_Weight-----------------------------
    
    cout<<"Weight of Uplinks\n";
    for(int k = 0 ;k<1;k++)
    {
        for(int i = 1 ;i<link;i++)
        {
            int up_wt = 0, down_wt = 0;
            for(int j = 1;j<deadline;j++)
            {
                up_wt += up_Tou[k][i][j];
                down_wt += down_Tou[k][i][j];
            }

            up_Weight[i] = (double)up_wt*fraction;
            down_Weight[i] = (double)down_wt*fraction;
            cout<<up_Weight[i]<<" ";
        }
        cout<<"\n\n";
    }
    
    cout<<"Weight of Downlinks"<<endl;
    for(int i = 1 ;i<link;i++)
    {
        cout<<down_Weight[i]<<" ";
    }
    
    cout<<endl;
	update_Buffer(deadline_lower, deadline_upper, up_Buffer, down_Buffer, up_Tou, down_Tou);

}
