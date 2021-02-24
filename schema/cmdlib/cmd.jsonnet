// This is the "command" schema ("cmd") used by appfwk applications.
//
// It governs the expected top-level structure of command objects.
//
// Its objects hold substructure as an "any" type which is expected to
// be interpreted by the next lower layer.

local moo = import "moo.jsonnet";
local s = moo.oschema.schema("dunedaq.cmdlib.cmd");

// A temporary schema construction context.
local cs = {
    data: s.any("Data",
                doc="An opaque object holding lower layer substructure"),

    // fixme: this should be an enum but I'm not allowed to make it so yet.
    cmdid: s.string("CmdId", pattern=moo.re.ident_only,
                    doc="The command name.  FIXME: this should be an enum!"),
    // cmdid: s.enum("CmdId", ["unknown","init","conf","start","stop","scrap","fini"],
    //                 "unknown", doc="The known command types"),
    
    isok: s.boolean("IsOk", doc="Outcome of the command: OK=true, NotOK=false"),
     
    descr: s.string("Result",
                    doc="The command name.  FIXME: this should be an enum!"),
 
 
    command: s.record("Command", [
        s.field("id", self.cmdid,
                doc="Identify the type of command"),
        s.field("data", self.data, optional=true,
                doc="Command data object with type-specific structure"),
    ], doc="Top-level command object structure"),

    cmdreply: s.record("CommandReply", [
        s.field("success", self.isok, 1, doc="Outcome of the command: OK=true, NotOK=false"),
        s.field("result", self.descr, doc="Textual result of the command"),
        s.field("data", self.data, optional=true,
                doc="Command reply data object with type-specific structure"),
    ], doc="Top-level command reply object structure"),
};

// Output a topologically sorted array.
moo.oschema.sort_select(cs)
