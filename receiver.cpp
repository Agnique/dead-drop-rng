
#include"util.hpp"
#include <signal.h>
#include <time.h>

char decode(int buf[8]);
char receive_packet();
clock_t begin;
clock_t end;

int main(int argc, char **argv)
{
	// Put your covert channel setup code here
    
	int contention = 0;
	int bit;
	int state = 0; 
    bool listening = 1;
    int iter=0;
	char ch;

	while(listening)
	{
		contention = probe_rdseed(5);
		nops(500);
		switch(state)
		{
			case 0:
			    if(contention > 0) state = 1;
				break;
            case 1:
			    if(contention > 0) state = 2;
				else state = 0;
				break;
            case 2:
                if(contention<1) state = 0;
                ch = receive_packet();
				if(ch!=-1) printf("%c",ch);				
				break;
		}
		
	}

	return 0;
}

char decode(int buf[8])
{
	char ch = 0;
	char m = 0x01;
    for(int i=7;i>-1;i--)
	{
		ch = ch | (buf[i]<<i);
	}
	return ch;
}


char receive_packet()
{
	char ch=-1;

    int buf[11]={0};
	int contention;
	int state = 0;
	int i = 0;
	int cnt = 0;
	int pb = 0;
	int iter;
	int idle = 0;

	contention = probe_rdseed(5);
	nops(500);
    int tmp=0;
	while(contention)
	{
		contention = probe_rdseed(5);
	    nops(500);
		tmp++;
	}
	if(tmp < 10) return -1;

	while(i<11)
	{
	    contention = probe_rdseed(5);
		nops(500);
		switch(state)
		{
			case 0:
			    if(contention > 0)
				{
					state = 1;
				}
				break;
            case 1:
			    if(contention > 0)
				{
					state = 3;
					iter = 0;
				}
				else state = 0;
				break;
            case 2:
			    if(contention < 1)
				{
					state = 0;
					idle = 0;
				}
				else state = 3;
				break;
            case 3:
			    if(contention < 1)
				{
					state = 2;
					buf[i] = iter>10?1:0;
					i++;
				}
				break;
		}
		iter++;
		idle++;
		if(idle>1000) break;
	}

	if(i!=11)
	{
		do_n_rdseed(60000);
		return ch;
	}

	if(buf[9]==1&&buf[10]==0)
	{
		//check PB
		for(int j=0;j<8;j++)
		{
            if(buf[j]==1) pb++;
			pb = pb%2==0?0:1;
		}
		if(pb==buf[8]) 
		{
			ch = decode(buf);
			nops(500);
		}
		else
		{
            do_n_rdseed(60000);
			//printf("wrong pb\n");
		} 
	} 
	else
	{
		do_n_rdseed(60000);
		//printf("wrong end bits\n");
	}

    return ch;
}

