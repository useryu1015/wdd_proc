#include <stdlib.h>

#include "zlog.h"
#include "cJSON.h"
#include "sys_config.h"
#include "srv_hardware.h"
#include "ws_dataflow.h"

cJSON *load_json_file(const char *json_file)
{
    const int FILE_BUF_SIZE = 1024 * 1024 * 20;
    FILE *fp;
    char *jsonstr, *ps, *pe;
    cJSON *json;

    if (!(jsonstr = (char *)malloc(FILE_BUF_SIZE)))
    {
        return NULL;
    }
    ps = jsonstr;
    pe = ps + FILE_BUF_SIZE;

    if (!(fp = fopen(json_file, "rt")))
    {
        zlog_warn("cann't open file '%s'", json_file);

        free(jsonstr);
        return NULL;
    }

    while (!feof(fp) && ps < pe)
    {
        int r = 1024 * 4;
        if (pe - ps < r)
            r = pe - ps;
        r = fread(ps, 1, r, fp);
        ps += r;
    }

    if (!feof(fp))
    {
        zlog_warn("file '%s' is too large, length: %d", json_file, ftell(fp));
        fclose(fp);
        free(jsonstr);
        return NULL;
    }

    zlog_info("load file '%s', length: %d", json_file, ftell(fp));
    fclose(fp);

    json = cJSON_Parse(jsonstr);
    if (!json)
    {
        zlog_warn("json parse failed: %s", jsonstr);

        free(jsonstr);
        return NULL;
    }
    // sys_logalert(E_S_CONF, EVENT_PROGRESS, APP_INITED, "parse %s failed", pname);

    free(jsonstr);

    return json;
}


void cJSON_ParseObjectInfo(const cJSON *json, hw_info_t *pinfo)
{
    struct cJSON *next;

    if (!json || !pinfo)
        return;

    for (next = json->child; NULL != next; next = next->next)
    {
        if (0 == cJSON_strcasecmp("id", next->string))
        {
            strncpy(pinfo->id, next->valuestring, sizeof(pinfo->id));
        }
        else if (0 == cJSON_strcasecmp("name", next->string))
        {
            strncpy(pinfo->name, next->valuestring, sizeof(pinfo->name));
        }
        else if (0 == cJSON_strcasecmp("disable", next->string))
        {
            pinfo->disable = (cJSON_True == next->type);
        }
        else if (0 == cJSON_strcasecmp("time", next->string))
        {
            strncpy(pinfo->time, next->valuestring, sizeof(pinfo->time));
        }
    }
}


void cJSON_ParseParamInfo(const cJSON* json, hw_cache_t* pval) {
	struct cJSON* next;

	if (!json || !pval)
		return;

	for (next = json->child; NULL != next; next = next->next) {
		if (0 == cJSON_strcasecmp("kks", next->string)) {
			strncpy(pval->kks, next->valuestring, sizeof(pval->kks));
		} else if (0 == cJSON_strcasecmp("vtype", next->string)) {
            pval->val_type = get_hw_type_by_name(next->valuestring);
		} else if (0 == cJSON_strcasecmp("room", next->string)) {
            strncpy(pval->room, next->valuestring, sizeof(pval->room));
		} else if (0 == cJSON_strcasecmp("port", next->string)) {
            pval->_GPIO.io = next->valueint;
		} else if (0 == cJSON_strcasecmp("rangeI", next->string)) {
			int i;
			struct cJSON* next_child;

			for (i = 0, next_child = next->child; i < 2 && NULL != next_child;
			++i, next_child = next_child->next) {
				pval->rangeI[i] = next_child->valueint;
			}
		} else if (0 == cJSON_strcasecmp("rangeD", next->string)) {
			int i;
			struct cJSON* next_child;

			for (i = 0, next_child = next->child; i < 2 && NULL != next_child;
			++i, next_child = next_child->next) {
				pval->rangeD[i] = next_child->valueint;
			}
		}
	}
}


void cJSON_ParseMainInfo(const cJSON *json)
{
    struct cJSON *next;
    if (!json)
        return;

    cJSON_ParseObjectInfo(json, &hw->hwInfo);       // titel
    zlog_info("Iint main id:%s name:%s", hw->hwInfo.id, hw->hwInfo.name);

    for (next = json->child; NULL != next; next = next->next)
    {
        if (0 == cJSON_strcasecmp(CONF_PARAM_FIELD, next->string))      // param
        {
            int i;
            struct cJSON *next_chd;

            for (i = 0, next_chd = next->child; i < hw->nChNum; ++i, next_chd = next_chd->next)
            {
                cJSON_ParseParamInfo(next_chd, hw->pCache[i]);
                // zlog_info("Init param kks:%s vtype:%s gpio:%d", hw->pCache[i]->kks, 
                //             get_hw_name_by_type(hw->pCache[i]->val_type), hw->pCache[i]->_GPIO.io);
                // zlog_info("rangeI: %d %d  rangeD:%d %d", hw->pCache[i]->rangeI[0], hw->pCache[i]->rangeI[1], -1, -1);
            }
        }
        if (0 == cJSON_strcasecmp(CONF_WEBSK_FIELD, next->string))      // websocket
        {
            
        }
    }


}

int load_configuration(const char* json_file) {
    cJSON* json = load_json_file(json_file);

    if (json) {
        // cJSON_ParseSinkInfo(json, &SINK);
        cJSON_ParseMainInfo(json);
        cJSON_Delete(json);

        return 0;
    } else {
        return -1;
    }
}

int conf_main_init()
{
    char filename[256];

    // xlian/conf/sink/dev_main.json
    sprintf(filename, "%s%s.%s", CONF_PATH, CONF_NAME, CONF_EXT);
    if (load_configuration(filename) < 0) {
        return -1;
    }

    return 0;
}