# -*- coding: <encoding name> -*- : # -*- coding: utf-8 -*-
import json
import sys
import os

cpp_headers = \
"""/*
* DON'T TOUCH!
* This file is generated by python script AUTOMATICALLY!
*
* Generated base on: {0}
* Type name: {1}
* Tester:    int {1}_tester();
*
* Json keep-word: 
    "__default_value_fields__": [], # Take value in .json file as the default value of cpp variable
    "__optional_fields__": [], # Not require to present to .json file, but always in cpp struct
    "__assign_type_fields__": {{"field":"cpp-type"}}, # Assign specal cpp-type of field, but not infer automatically as default
    "__assign_set_lists__": [], # Take list in .json file as std::set<>, but not std::vector<> as default
    "__comment__xxx":"", # Add comment line
    "__sqlite_capable__":"", # enable sqlite tableIO autogen
    "__sqlite_primary__":"", # assign the primary key of sqlite, if not assigned, first existing col of [ 'id', 'date', 'mktCode', 'datetime', 'code'] will be assigned automatically.
* Script author: ChinSaiki<chinsaiki@outlook.com>
*/
#pragma once

"""

json_cpp_headers = cpp_headers + \
"""
#include <assert.h>
#include "common/s4json_util.h"
#include "common/s4logger.h"
#include "types/s4type.h"

#include <set>
#include <list>
#include <vector>
"""

keep_words = [
    '__default_value_fields__', 
    '__optional_fields__', 
    '__assign_type_fields__', 
    '__assign_set_lists__', 
    '__sqlite_capable__', 
    '__sqlite_primary__'
]

def determin_value_type(value):
    if isinstance(value, str):
        return "std::string"
    elif isinstance(value, bool):
        return "bool"
    elif isinstance(value, int):
        return "int"
    elif isinstance(value, float):
        return "double"

def determin_default_value(value):
    if isinstance(value, str):
        return '"%s"' % value
    elif isinstance(value, bool):
        return "true" if value else "false"
    elif isinstance(value, int):
        return "%d" % value
    elif isinstance(value, float):
        return "%.f" % value

def dict_to_struct(cpp_vari, json_vari, type_name, json_dict, namespace_list = [], json_vari_suffix=''):
    main_str = ["struct {} {{".format(type_name)]
    from_str = []
    to_str = []
    if '__optional_fields__' in json_dict:
        __optional_fields__ = json_dict['__optional_fields__']
    else:
        __optional_fields__ = []
    if '__default_value_fields__' in json_dict:
        __default_value_fields__ = json_dict['__default_value_fields__']
    else:
        __default_value_fields__ = []
    __optional_fields__.extend(__default_value_fields__)    # field with default value is always optional field

    if '__assign_type_fields__' in json_dict:
        __assign_type_fields__ = json_dict['__assign_type_fields__']
        print("use __assign_type_fields__ = {}".format(__assign_type_fields__))
    else:
        __assign_type_fields__ = {}

    if '__assign_set_lists__' in json_dict:
        __assign_set_lists__ = json_dict['__assign_set_lists__']
        print("use __assign_set_lists__ = {}".format(__assign_set_lists__))
    else:
        __assign_set_lists__ = []
    
        
    for key_name in json_dict:
        if key_name in keep_words:
            continue

        key_value = json_dict[key_name]

        if key_name.find("__comment__")==0:
            main_str.append("\t//"+key_value)
            continue

        # if key_value is None:
        #     key_type = 'bool'
        #     key_name = key_name + "_is_null"
        #     main_str.append("\t{} {};\t//\t{}".format(key_type, key_name, key_value))

        #     from_str.append('{0}.{2} = {1}{3}["{2}"].is_null();'.format(cpp_vari, json_vari, key_name, json_vari_suffix))
        # el
        if isinstance(key_value, (str, int, float, bool)):
            if key_name in __assign_type_fields__:
                key_type = __assign_type_fields__[key_name]
                print("use assigned type %s = %s" % (key_name, key_type) )
            else:
                key_type = determin_value_type(key_value)
            
            if key_name in __default_value_fields__:
                main_str.append("\t{} {} = ({}){};".format(key_type, key_name, key_type, determin_default_value(key_value)))
            else:
                main_str.append("\t{} {};\t//\t{}".format(key_type, key_name, key_value))

            from_str.append('try{')
            from_str.append('\t{0}.{2} = {1}{4}.at("{2}").get<{3}>();'.format(cpp_vari, json_vari, key_name, key_type, json_vari_suffix))
            if key_name not in __optional_fields__:
                from_str.append("}catch(const std::exception& e){")
                from_str.append('\tERR("{{:}} not found in json! e={{:}}", "{}", e.what());'.format(key_name))
                from_str.append('\tthrow e;')
            else:
                from_str.append("}catch(...){")
            from_str.append("}")

            to_str.append('{}["{}"] = {}.{};'.format(json_vari, key_name, cpp_vari, key_name))
        elif isinstance(key_value, dict):
            sub_json_vari = json_vari+"_"+key_name
            sub_type = key_name+"_t"
            sub_str, sub_from, sub_to = dict_to_struct(cpp_vari+"."+key_name, sub_json_vari, sub_type, key_value, namespace_list+[sub_type])

            sub_str = ["\t"+x for x in sub_str]
            sub_str.append("\t{}_t {};".format(key_name, key_name))
            main_str.extend(sub_str)

            from_str.append('const nlohmann::json& {} = {}{}["{}"];'.format(sub_json_vari, json_vari, json_vari_suffix, key_name))
            from_str.extend(sub_from)

            to_str.append('nlohmann::json {};'.format(sub_json_vari))
            to_str.extend(sub_to)
            to_str.append('{}["{}"] = {};'.format(json_vari, key_name, sub_json_vari))
        elif isinstance(key_value, list):
            if len(key_value)==0:
                print("unable to determin type of list({}:{})".format(key_name, key_value))
            else:
                key_value = key_value[0]
                if isinstance(key_value, (str, int, float, bool) or key_name in __assign_type_fields__):
                    sub_json_vari = json_vari+"_"+key_name

                    if key_name in __assign_type_fields__:
                        key_type = __assign_type_fields__[key_name]
                        print("use assigned type %s = std::vector<%s>" % (key_name, key_type) )
                    else:
                        key_type = determin_value_type(key_value)

                    if key_name in __assign_set_lists__:
                        main_str.append("\tstd::set<{}> {};\t//\t{}".format(key_type, key_name, key_value))
                    else:
                        main_str.append("\tstd::vector<{}> {};\t//\t{}".format(key_type, key_name, key_value))
                    
                    sub_str = []
                    sub_str.append('try{')
                    sub_str.append('\tconst nlohmann::json& {} = {}{}.at("{}");'.format(sub_json_vari, json_vari, json_vari_suffix, key_name))
                    sub_str.append('\tfor(auto& {0}_x: {0}.items()){{'.format(sub_json_vari))
                    if key_name in __assign_set_lists__:
                        sub_str.append('\t\t{}.{}.insert({}_x.value().get<{}>());'.format(cpp_vari, key_name, sub_json_vari, key_type))
                    else:
                        sub_str.append('\t\t{}.{}.push_back({}_x.value().get<{}>());'.format(cpp_vari, key_name, sub_json_vari, key_type))
                    sub_str.append('\t}')
                    if key_name not in __optional_fields__:
                        sub_str.append("}catch(const std::exception& e){")
                        sub_str.append('\tERR("parse field {{:}} fail! e={{:}}", "{}", e.what());'.format(key_name))
                        sub_str.append('\tthrow e;')
                    else:
                        sub_str.append("}catch(...){")
                    sub_str.append("}")


                    from_str.extend(sub_str)

                    to_str.append('{}["{}"] = {}.{};'.format(json_vari, key_name, cpp_vari, key_name))
                elif isinstance(key_value, dict):
                    sub_json_vari = json_vari+"_"+key_name
                    sub_cpp_vari = cpp_vari.replace('.', '_')+"_"+key_name
                    sub_type = key_name+"_t"
                    sub_str, sub_from, sub_to = dict_to_struct(sub_cpp_vari, sub_json_vari, sub_type, key_value, namespace_list+[sub_type], ".value()")

                    sub_str = ["\t"+x for x in sub_str]
                    main_str.extend(sub_str)
                    main_str.append("\tstd::vector<{}::{}> {};".format("::".join(namespace_list), key_name+"_t", key_name))

                    sub_str = []
                    sub_str.append('try{')
                    sub_str.append('\tconst nlohmann::json& {0} = {1}{3}.at("{2}");'.format(sub_json_vari, json_vari, key_name, json_vari_suffix))
                    sub_str.append('\tfor(auto& {0}_x: {0}.items()){{'.format(sub_json_vari))
                    sub_str.append('\t\t{}::{} {};'.format("::".join(namespace_list), sub_type, sub_cpp_vari))
                    sub_from = ["\t\t"+x for x in sub_from]
                    sub_from = [x.replace(sub_json_vari, sub_json_vari+'_x') for x in sub_from]
                    sub_str.extend(sub_from)
                    sub_str.append('\t\t{}.{}.emplace_back({});'.format(cpp_vari, key_name, sub_cpp_vari))
                    sub_str.append('\t}')
                    if key_name not in __optional_fields__:
                        sub_str.append("}catch(const std::exception& e){")
                        sub_str.append('\tERR("parse field {{:}} fail! e={{:}}", "{}", e.what());'.format(key_name))
                        sub_str.append('\tthrow e;')
                    else:
                        sub_str.append("}catch(...){")
                    sub_str.append("}")

                    from_str.extend(sub_str)

                    sub_str = []
                    sub_str.append('{}["{}"] = nlohmann::json();'.format(json_vari, key_name))
                    sub_str.append('for(const auto& {}_x: {}.{}){{'.format(sub_cpp_vari, cpp_vari, key_name))
                    sub_str.append('\tjson {};'.format(sub_json_vari))
                    sub_to = ["\t"+x for x in sub_to]
                    sub_to = [x.replace(sub_cpp_vari, sub_cpp_vari+'_x') for x in sub_to]
                    sub_str.extend(sub_to)
                    sub_str.append('\t{}["{}"].push_back({});'.format(json_vari, key_name, sub_json_vari))
                    sub_str.append('}')

                    to_str.extend(sub_str)
                else:
                    print("unsupported list type for {}:{}".format(key_name, key_value))
                    exit(-1)
        else:
            print("unsupported type for {}:{}".format(key_name, key_value))
            exit(-1)

    main_str.append("};")
    return main_str, from_str, to_str

def get_eq(type_name, json_dict):
    cols_list = []
    if '__assign_type_fields__' in json_dict:
        __assign_type_fields__ = json_dict['__assign_type_fields__']
        print("use __assign_type_fields__ = {}".format(__assign_type_fields__))
    else:
        __assign_type_fields__ = {}
        
    for key_name in json_dict:
        if key_name in keep_words or key_name.find("__comment__")==0:
            continue

        key_value = json_dict[key_name]

        if isinstance(key_value, (str, int, float, bool)):
            cols_list.append(key_name)
        elif isinstance(key_value, dict):
            print("unsupported type for <dict> {}:{}".format(key_name, key_value))
            return ''
        elif isinstance(key_value, list):
            print("unsupported type for <list> {}:{}".format(key_name, key_value))
            return ''
            # if len(key_value)==0:
            # else:
            #     key_value = key_value[0]
            #     if isinstance(key_value, (str, int, float, bool) or key_name in __assign_type_fields__):
            #     elif isinstance(key_value, dict):
            #     else:
            #         print("unsupported list type for {}:{}".format(key_name, key_value))
            #         return ''
        else:
            print("unsupported type for {}:{}".format(key_name, key_value))
            return ''


    cpp_ueq = '''
	bool operator !=(const {}& d) const
	{{
		return !((*this)==d);
	}}
'''.format(type_name)
    
    eq_list = []
    for col in cols_list:
        eq_list.append('{0} == d.{0}'.format(col))

    cpp_eq = '''
	bool operator ==(const {0}& d) const
	{{
		if ({1})
		{{
			return true;
		}}
		return false;
	}}
'''.format(type_name, ' &&\n\t\t\t'.join(eq_list))
    return cpp_eq + cpp_ueq


if __name__ == "__main__":
    print("Current working path={}".format(os.getcwd()))

    if len(sys.argv)==1:
        print("Need at least 1 arg:")
        print("  {} <json file>  [<output file>]".format(sys.argv[0]))
        exit(1)

    src_json = sys.argv[1]
    if len(sys.argv)>=3:
        tgt_cpp = sys.argv[2]
    else:
        tgt_cpp = src_json.replace(".json", ".h")

    if len(sys.argv)>=4:
        struct_only = sys.argv[3]
    else:
        struct_only = False

    print("reading json file:{}".format(src_json))
    with open(src_json, 'r', encoding='UTF-8') as f:
        text = f.readlines()
    # print(text)
    text = "".join(text)
    json_instance = json.loads(text)

    # print("read json OK:\n{}".format(json.dumps(json_instance, indent=4, separators=(',', ': '))))

    print("read json base type={}".format(type(json_instance)))
    if not isinstance(json_instance, dict):
        print("error:only support base type==dict for now!")
        exit(0)

    type_name = os.path.basename(src_json)
    type_name = type_name.split(".")[0]
    print("Creating Cpp file for type={}".format(type_name))

    cpp_v = type_name + "_var"
    json_v = "json_var"
    cpp_struct, cpp_from_str, cpp_to_str = dict_to_struct(cpp_v, json_v, type_name, json_instance, [type_name])
    output = []

    json_cpp_headers = json_cpp_headers.format(src_json, type_name)
    output.append(json_cpp_headers)

    output.append("namespace S4 {\n")

    print("/* type */")
    output.append("/* type */")
    cpp_struct = "\n".join(cpp_struct)
    # print(cpp_struct)
    output.append(cpp_struct[:-2])

    if not struct_only:
        print("/* from json */")
        output.append("\t/* from json */")
        cpp_from = ['\tstatic bool from_json(const nlohmann::json& {}, {}& {}){{'.format(json_v, type_name, cpp_v)]
        cpp_from.append("\t\ttry{")
        cpp_from_str = ['\t\t\t'+x for x in cpp_from_str]
        cpp_from.extend(cpp_from_str)
        cpp_from.append("\t\t}catch (const std::exception& e){")
        cpp_from.append('\t\t\tERR("parse json {{:}} \\nfail:{{:}}", {}.dump(4), e.what());'.format(json_v))
        cpp_from.append("\t\t\treturn false;")
        cpp_from.append("\t\t}")
        cpp_from.append("\t\treturn true;")
        cpp_from.append("\t}")
        cpp_from = "\n".join(cpp_from)
        # print(cpp_from)
        output.append(cpp_from)

        print("/* to json */")
        output.append("\t/* to json */")
        cpp_to = ['\tstatic bool to_json(nlohmann::json& {}, const {}& {}){{'.format(json_v, type_name, cpp_v)]
        cpp_to.append("\t\ttry{")
        cpp_to_str = ['\t\t\t'+x for x in cpp_to_str]
        cpp_to.extend(cpp_to_str)
        cpp_to.append("\t\t}catch (const std::exception& e){")
        cpp_to.append('\t\tERR("to json {{:}} \\nfail:{{:}}", {}.dump(4), e.what());'.format(json_v))
        cpp_to.append("\t\t\treturn false;")
        cpp_to.append("\t\t}")
        cpp_to.append("\t\treturn true;")
        cpp_to.append("\t}")
        cpp_to = "\n".join(cpp_to)
        # print(cpp_to)
        output.append(cpp_to)

        cpp_eq = get_eq(type_name, json_instance)
        output.append(cpp_eq)

    output.append("}};// struct {}".format(type_name))

    output.append("} // namespace S4")


    if not struct_only:
        cpp_json_text = text.replace('"', '\\"').replace("\n","").replace("\r","")
        test_source = """

        /* Tester */
        inline int {3}_tester() {{

            //std::ifstream i("{0}/{1}");
            std::string i("{5}");
            nlohmann::json {2};
            //i >> {2}; //from file
            {2} = nlohmann::json::parse(i);  //from string

            S4::{3} {4};

            if(!S4::{3}::from_json({2}, {4})){{
                INFO("S4::{3}::from_json fail!");
                return -1;
            }}

            nlohmann::json j_out;
            if(!S4::{3}::to_json(j_out, {4})){{
                INFO("S4::{3}::to_json fail!");
                return -1;
            }}

            INFO("{{:}}", j_out.dump(4));

            return 0;
        }}

        """.format(os.getcwd().replace("\\", "/"), 
        src_json.replace("\\", "/"), 
        json_v,
        type_name, cpp_v,
        cpp_json_text
        )
        # print(test_source)
        output.append(test_source)

    output = '\n'.join(output)
    if tgt_cpp is not None:
        with open(tgt_cpp, 'w+') as fo:
            fo.write(output)

    print("output cpp .h = {}".format(tgt_cpp))