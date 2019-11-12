
#include"util.hpp"

int main(int argc, char **argv)
{

    // N_prime = 8, T_prime = 1380, one_rdseed = 189, T_refill=nops(550), T_prime=nops(190)
	//CYCLES c;
	//double avg=0;
    //for(int i=0;i<1;i++)
	//{
	//	nops(1000000);
    //    c = measure_prime_time();
    //    avg += c;
	//}
	//printf("prime time:%f\n",avg/1000);
	
    //double cnt=0;
	//int a;
	//a = num_valid_rdseed();
	//printf("num rdseed: %d\n",a);

	//a = test_n_rdseed(8);
	//printf("contention:%d\n",a);

    //avg=0;
	//for(int i=0;i<1;i++)
	//{
	//	nops(1000000);
	//	c = measure_one_rdseed_time();
	//	avg += c;
	//}
	//printf("one rdseed time: %f\n",avg/1000);
	
    //c = nops_time(3);
    //printf("nops:%d\n",c);

    double a;
    for(uint32_t i=200;i<1000;i+=100)
    {
        double cnt=0;
        for(int j=0;j<1000;j++)
        {
            nops(100000);
            a = probe_rdseed(3);
            nops(i);
            a = probe_rdseed(3);
            if(a==1) cnt++;

        }
        a = cnt/1000;
        printf("nops:%d failure rate:%f\n",i,a);
    }
    for(int i=1000;i<20000;i+=1000)
    {
        CYCLES c;
        c = nops_time(i);
        printf("nops:%d time:%d\n",i,c);
    }


	return 0;
}