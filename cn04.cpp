#include <iostream>
using namespace std;

class TCPConnection
{
private:
    int cwnd;
    int ssthresh;
    int rtt;
    bool nw;
    int dataCount;
    bool fastRec;
    int lastAck;
public:
    TCPConnection(int c, int s, int r = 0, bool n = false)
    {
        cwnd = c;
        ssthresh = s;
        rtt = r;
        nw = n;
        dataCount = 0;
        fastRec = false;
        lastAck = 0;
    }
    int GetCwnd()
    {
        return cwnd;
    }
    int GetSsthresh()
    {
        return ssthresh;
    }
    int GetRtt()
    {
        return rtt;
    }
    string GetMechanism()
    {
        if(fastRec)
            return "Fast Recovery";
        else if(cwnd < ssthresh)
            return "Slow Start";
        else
            return "Congestion Avoidance";
    }
    void SendData()
    {
        if(cwnd < ssthresh)
            cwnd *=2;
        else
            cwnd++;
        dataCount += cwnd;
    }
    void onPockeLoss()
    {
        if(!nw || !fastRec)
        {
            ssthresh = cwnd / 2;
            cwnd = ssthresh;
            fastRec = true;
        }
    }
    void onRTTUpdate(int r)
    {
        rtt = r;
    }
    void onSelectiveAck(int ackNum)
    {
        if(fastRec)
        {
            if(ackNum > lastAck)
                lastAck = ackNum;
            if(ackNum < dataCount)
                cwnd++;
            else
            {
                if(cwnd > ssthresh)
                    cwnd = ssthresh;
                fastRec = false;
            }
        }
    }
};

int main()
{
    TCPConnection c1(1,10),c2(7,12),c3(1,10,0,true),c4(7,12,0,true);
    c1.SendData();
    cout<<c1.GetCwnd()<<" "<<c1.GetSsthresh()<<" "<<c1.GetRtt()<<" "<<c1.GetMechanism()<<endl;
    c2.SendData();
    c2.onRTTUpdate(50);
    cout<<c2.GetCwnd()<<" "<<c2.GetSsthresh()<<" "<<c2.GetRtt()<<" "<<c2.GetMechanism()<<endl;
    c1.SendData();
    c1.onRTTUpdate(100);
    cout<<c1.GetCwnd()<<" "<<c1.GetSsthresh()<<" "<<c1.GetRtt()<<" "<<c1.GetMechanism()<<endl;
    c1.onPockeLoss();
    cout<<c1.GetCwnd()<<" "<<c1.GetSsthresh()<<" "<<c1.GetRtt()<<" "<<c1.GetMechanism()<<endl;
    c1.SendData();
    cout<<c1.GetCwnd()<<" "<<c1.GetSsthresh()<<" "<<c1.GetRtt()<<" "<<c1.GetMechanism()<<endl;
    
    c3.SendData();
    cout<<c3.GetCwnd()<<" "<<c3.GetSsthresh()<<" "<<c3.GetRtt()<<" "<<c3.GetMechanism()<<endl;
    c4.SendData();
    c4.onRTTUpdate(50);
    cout<<c4.GetCwnd()<<" "<<c4.GetSsthresh()<<" "<<c4.GetRtt()<<" "<<c4.GetMechanism()<<endl;
    c3.SendData();
    c3.onRTTUpdate(100);
    cout<<c3.GetCwnd()<<" "<<c3.GetSsthresh()<<" "<<c3.GetRtt()<<" "<<c3.GetMechanism()<<endl;
    c3.onPockeLoss();
    cout<<c3.GetCwnd()<<" "<<c3.GetSsthresh()<<" "<<c3.GetRtt()<<" "<<c3.GetMechanism()<<endl;
    c3.onSelectiveAck(4);
    cout<<c3.GetCwnd()<<" "<<c3.GetSsthresh()<<" "<<c3.GetRtt()<<" "<<c3.GetMechanism()<<endl;
    c3.onSelectiveAck(6);
    cout<<c3.GetCwnd()<<" "<<c3.GetSsthresh()<<" "<<c3.GetRtt()<<" "<<c3.GetMechanism()<<endl;
    c3.SendData();
    cout<<c3.GetCwnd()<<" "<<c3.GetSsthresh()<<" "<<c3.GetRtt()<<" "<<c3.GetMechanism()<<endl;
    return 0;
}