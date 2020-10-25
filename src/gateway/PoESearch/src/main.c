#include "goahead.h"
#include "config.h"

void PoEQueryRecv(Webs *wp);

int main(int argc, char **argv)
{
	int ret = 0;
	char route_dir[64];
	char endpoint[128];
	

	
	pthread_t tid;
	
	memset(&config_param, 0, sizeof(config_param));

	//ret = read_config(argv[1], &config_param);
	ret = read_config("config.ini", &config_param);
	if(ret)
	{
		return ret;
	}

	#if 1
	memset(route_dir, 0, sizeof(route_dir));
	memset(endpoint, 0, sizeof(endpoint));
	sprintf(route_dir, "%s/%s", config_param.dir, "route.txt");
	sprintf(endpoint, "http://%s:%s", config_param.ipaddr, config_param.port);
	//sprintf(endpoint, "http://%s:80, https://%s:443", config_param.ipaddr, config_param.ipaddr);
	//printf("route_dir:%s\r\n", route_dir);
	//printf("endpoint:%s\r\n", endpoint);
	
    if (websOpen(config_param.doc, route_dir) < 0) {
        error("Can't open the web server runtime");
        return 0;
    }
    //if (websListen("http://*:80,https://*:443") < 0) {
    if (websListen(endpoint) < 0) {
        error("Can't listen on port 80");
        return 0;
    }

	websDefineAction("PoEQuery", PoEQueryRecv);
	#if 0
	websDefineAction("event_receiver", iov_event_recv);
	websDefineJst("MakeDevsAttrib", getDevsAttrib);
	websDefineJst("MakeEventAttrib", getEventAttrib);
	websDefineJst("MakeDevsMapAttrib", getDevsMapAttrib);
	#endif
	
    websServiceEvents(NULL);
    websClose();
	#endif
    return 0;
}

