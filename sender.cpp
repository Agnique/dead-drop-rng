
#include"util.hpp"
#include <signal.h>
#include <time.h>

void send(int bit);
void send_packet(char ch);
void send_msg(char* c);
int get_len(char* c);
clock_t begin;
clock_t end;

int main(int argc, char **argv)
{
	// Put your covert channel setup code here

	printf("Please type a message.\n");
 
	bool sending = true;
	char msg[100];
    double send_time;
	int msg_len;
    char c[5]={'a','b','c','d','\n'};

	while(sending)
	{
		gets(msg);
		msg_len = get_len(msg);
		begin = clock();
		send_msg(msg);
		end = clock();
		send_time = (double)(end-begin)/CLOCKS_PER_SEC;
		printf("speed:%f bps\n",(double)msg_len*8/send_time);
	}
    
	printf("Sender finished.\n");

	return 0;
}

void send(int bit)
{
	clock_t begin;
	clock_t end;
	
	nops(10000);
	if(bit==0)
	{   
		do_n_rdseed(100);
	}
	else
	{
        do_n_rdseed(300);
	}
}

void send_packet(char ch)
{
	// starter bit
	send(1);

    int buf[9];
	int pb = 0;
	int i;
	for(i=0;i<8;i++)
	{
		buf[i] = (ch >> i) & 0x01;
		if(buf[i]==1) pb++;
	}
	pb = pb%2==0?0:1;
	buf[i] = pb;
    for(int i=0;i<9;i++)
    {
        send(buf[i]);
    }
    // stop bits
	send(1);
	send(0);

}

void send_msg(char* c)
{
	int state = 0;
	int contention;
	int i=-1;
	int it;
	while(i==-1 || c[i]!=0)
	{
		switch (state)
		{
		case 0: // finish state
			contention = probe_rdseed(2);
		    nops(500);
			if(contention == 0)
			{
				i++;
				state = 1;
			}
			break;		
		case 1: // send state
		    nops(5000);
		    send_packet(c[i]);
			//nops(200);
			printf("sending %c\n",c[i]);
		    contention = probe_rdseed(2);
		    nops(500);
			contention = probe_rdseed(2);
			nops(500);
			it = 0;
			while(contention)
			{
				contention = probe_rdseed(2);
				nops(500);
				it++;
			}
			if(it>0)
			{
				nops(100000);
				printf("wrong, it:%d\n",it);
				state = 1;
			}
			else state = 0;
			break;
		}
        
	}

}

int get_len(char* c)
{
	int i=0;
	while(c[i]!=0)
	{
		i++;
	}
    c[i]='\n';
	c[i+1] = '\0';

	return i;
}


