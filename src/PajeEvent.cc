#include "PajeEvent.h"

// must follow PajeEventId definition
const char *PajeEventNames[] = {
    "PajeStartTrace",
    "PajeDefineContainerType",
    "PajeDefineEventType",
    "PajeDefineStateType",
    "PajeDefineVariableType",
    "PajeDefineLinkType",
    "PajeDefineEntityValue",
    "PajeCreateContainer",
    "PajeDestroyContainer",
    "PajeNewEvent",
    "PajeSetState",
    "PajePushState",
    "PajePopState",
    "PajeSetVariable",
    "PajeAddVariable",
    "PajeSubVariable",
    "PajeStartLink",
    "PajeEndLink"
};

// must follow PajeFieldId definition
const char *PajeFieldNames[] = {
    "EventId",
    "Time",
    "Name",
    "Alias",
    //"ContainerType",
    //"EntityType",
    "Type",
    "Container",
    "StartContainerType",
    "EndContainerType",
    "StartContainer",
    "EndContainer",
    "Color",
    "Value",
    "Key",
    "File",
    "Line"
};
const char *PajeOldFieldNames[] = {
    "",
    "",
    "NewName", // Container
    "NewType", // NewValue, NewContainer
    //"",
    //"Type",
    "NewContainerType",
    "",
    "SourceContainerType",
    "DestContainerType",
    "SourceContainer",
    "DestContainer",
    "",
    "",
    "",
    "FileName",
    "LineNumber"
};

const char *PajeOld1FieldNames[] = {
    "",
    "",
    "", // Container
    "NewValue", // NewContainer
    //"",
    //"",
    "ContainerType",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    ""
};

const char *PajeOld2FieldNames[] = {
    "",
    "",
    "", // Container
    "NewContainer", // NewContainer
    //"",
    //"",
    "EntityType",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    ""
};

// NSArray *PajeExtraFieldNames;

PajeFieldId obligatoryFieldIds[][10] = {
    /*PajeStartTrace         */{ NoFieldId },
    /*PajeDefineContainerType*/{ PajeNameFieldId, PajeTypeFieldId, NoFieldId },
    /*PajeDefineEventType    */{ PajeNameFieldId, PajeTypeFieldId, NoFieldId },
    /*PajeDefineStateType    */{ PajeNameFieldId, PajeTypeFieldId, NoFieldId },
    /*PajeDefineVariableType */{ PajeNameFieldId, PajeTypeFieldId, NoFieldId },
    /*PajeDefineLinkType     */{ PajeNameFieldId, PajeTypeFieldId, PajeStartContainerTypeFieldId, PajeEndContainerTypeFieldId, NoFieldId },
    /*PajeDefineEntityValue  */{ PajeNameFieldId, PajeTypeFieldId, NoFieldId },
    /*PajeCreateContainer    */{ PajeTimeFieldId, PajeNameFieldId, PajeTypeFieldId, PajeContainerFieldId, NoFieldId },
    /*PajeDestroyContainer   */{ PajeTimeFieldId, PajeNameFieldId, PajeTypeFieldId, NoFieldId },
    /*PajeNewEvent           */{ PajeTimeFieldId, PajeTypeFieldId, PajeContainerFieldId, PajeValueFieldId, NoFieldId },
    /*PajeSetState           */{ PajeTimeFieldId, PajeTypeFieldId, PajeContainerFieldId, PajeValueFieldId, NoFieldId },
    /*PajePushState          */{ PajeTimeFieldId, PajeTypeFieldId, PajeContainerFieldId, PajeValueFieldId, NoFieldId },
    /*PajePopState           */{ PajeTimeFieldId, PajeTypeFieldId, PajeContainerFieldId, PajeValueFieldId, NoFieldId },
    /*PajeSetVariable        */{ PajeTimeFieldId, PajeTypeFieldId, PajeContainerFieldId, PajeValueFieldId, NoFieldId },
    /*PajeAddVariable        */{ PajeTimeFieldId, PajeTypeFieldId, PajeContainerFieldId, PajeValueFieldId, NoFieldId },
    /*PajeSubVariable        */{ PajeTimeFieldId, PajeTypeFieldId, PajeContainerFieldId, PajeValueFieldId, NoFieldId },
    /*PajeStartLink          */{ PajeTimeFieldId, PajeTypeFieldId, PajeContainerFieldId, PajeValueFieldId, PajeStartContainerFieldId, PajeKeyFieldId, NoFieldId },
    /*PajeEndLink            */{ PajeTimeFieldId, PajeTypeFieldId, PajeContainerFieldId, PajeValueFieldId, PajeEndContainerFieldId, PajeKeyFieldId, NoFieldId }
};

PajeFieldId optionalFieldIds[][5] = {
    /*PajeStartTrace         */{ NoFieldId },
    /*PajeDefineContainerType*/{ PajeAliasFieldId, NoFieldId },
    /*PajeDefineEventType    */{ PajeAliasFieldId, NoFieldId },
    /*PajeDefineStateType    */{ PajeAliasFieldId, NoFieldId },
    /*PajeDefineVariableType */{ PajeAliasFieldId, NoFieldId },
    /*PajeDefineLinkType     */{ PajeAliasFieldId, NoFieldId },
    /*PajeDefineEntityValue  */{ PajeAliasFieldId, PajeColorFieldId, NoFieldId },
    /*PajeCreateContainer    */{ PajeAliasFieldId, NoFieldId },
    /*PajeDestroyContainer   */{ NoFieldId },
    /*PajeNewEvent           */{ PajeFileFieldId, PajeLineFieldId, NoFieldId },
    /*PajeSetState           */{ PajeFileFieldId, PajeLineFieldId, NoFieldId },
    /*PajePushState          */{ PajeFileFieldId, PajeLineFieldId, NoFieldId },
    /*PajePopState           */{ PajeFileFieldId, PajeLineFieldId, NoFieldId },
    /*PajeSetVariable        */{ NoFieldId },
    /*PajeAddVariable        */{ NoFieldId },
    /*PajeSubVariable        */{ NoFieldId },
    /*PajeStartLink          */{ PajeFileFieldId, PajeLineFieldId, NoFieldId },
    /*PajeEndLink            */{ PajeFileFieldId, PajeLineFieldId, NoFieldId }
};


// PajeEventId pajeEventIdFromName(const char *name)
// {
//     PajeEventId i;
//     for (i = PajeStartTraceEventId; i < PajeUnknownEventId; i++) {
//         if (strcmp(name, PajeEventNames[i]) == 0) {
//             return i;
//         }
//     }
//     return -1;
// }

// PajeFieldId pajeFieldIdFromName(const char *name)
// {
//     PajeEventId i;
//     for (i = 0; i < PajeFieldIdCount; i++) {
//         if (strcmp(name, [PajeFieldNames[i] cString]) == 0) {
//             return i;
//         }
//     }
//     for (i = 0; i < PajeFieldIdCount; i++) {
//         if (strcmp(name, [PajeOldFieldNames[i] cString]) == 0
//          || strcmp(name, [PajeOld1FieldNames[i] cString]) == 0
//          || strcmp(name, [PajeOld2FieldNames[i] cString]) == 0) {
//             return i;
//         }
//     }
//     for (i = 0; i < [PajeUserFieldNames count]; i++) {
//         if (strcmp(name, [[PajeUserFieldNames objectAtIndex:i] cString]) == 0) {
//             return i + PajeFieldIdCount;
//         }
//     }
//     [PajeUserFieldNames addObject:[NSString stringWithCString:name]];
//     return i + PajeFieldIdCount;
// }

// NSString *pajeFieldNameFromId(PajeFieldId fieldId)
// {
//     if (fieldId < PajeFieldIdCount) {
//         return PajeFieldNames[fieldId];
//     } else {
//         return [PajeUserFieldNames objectAtIndex:fieldId - PajeFieldIdCount];
//     }
// }


// PajeFieldType pajeFieldTypeFromName(const char *name)
// {
//     if (strcmp(name, "int")    == 0) return PajeIntFieldType;
//     if (strcmp(name, "hex")    == 0) return PajeHexFieldType;
//     if (strcmp(name, "date")   == 0) return PajeDateFieldType;
//     if (strcmp(name, "double") == 0) return PajeDoubleFieldType;
//     if (strcmp(name, "string") == 0) return PajeStringFieldType;
//     if (strcmp(name, "color")  == 0) return PajeColorFieldType;
//     return PajeUnknownFieldType;
// }
