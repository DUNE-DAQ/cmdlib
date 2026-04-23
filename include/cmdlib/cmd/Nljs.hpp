/*
 * This file is 100% generated.  Any manual edits will likely be lost.
 *
 * This contains functions struct and other type definitions for shema in 
 * namespace dunedaq::cmdlib::cmd to be serialized via nlohmann::json.
 */
#ifndef DUNEDAQ_CMDLIB_CMD_NLJS_HPP
#define DUNEDAQ_CMDLIB_CMD_NLJS_HPP

// My structs
#include "cmdlib/cmd/Structs.hpp"


#include <nlohmann/json.hpp>

namespace dunedaq::cmdlib::cmd {

    using data_t = nlohmann::json;
    
    inline void to_json(data_t& j, const Command& obj) {
        j["id"] = obj.id;
        j["data"] = obj.data;
    }
    
    inline void from_json(const data_t& j, Command& obj) {
        if (j.contains("id"))
            j.at("id").get_to(obj.id);    
        obj.data = j.at("data");
    }
    
    inline void to_json(data_t& j, const CommandReply& obj) {
        j["success"] = obj.success;
        j["result"] = obj.result;
        j["appname"] = obj.appname;
        j["data"] = obj.data;
    }
    
    inline void from_json(const data_t& j, CommandReply& obj) {
        if (j.contains("success"))
            j.at("success").get_to(obj.success);    
        if (j.contains("result"))
            j.at("result").get_to(obj.result);    
        if (j.contains("appname"))
            j.at("appname").get_to(obj.appname);    
        obj.data = j.at("data");
    }
    
} // namespace dunedaq::cmdlib::cmd

#endif // DUNEDAQ_CMDLIB_CMD_NLJS_HPP