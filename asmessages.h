/*
 *      Author: Hillel medioni
 *      This header only present error message for take from here and add param
 *      with sprintf to error client message
 */

#ifndef ASMESSAGES_H_
#define ASMESSAGES_H_

#define MAX_LENGTH_ERROR_MESSAGE 80

#define UNDEFINED_COMMAND "Undefined command name"
#define MISSING_PARAMETER "Missing parameter"
#define TO_MANY_PARAMETER "Too many parameters"
#define LABEL_UNKNOWN "Label unknown error"
#define LABEL_ALREADY_EXIST "Label already exist in .as"
#define LABEL_SYNTAX_ERROR "Label does not meet the criteria"
#define EXTERNAL_ALREADY_EXIST "External already exist in .as"
#define EXTERNAL_USED_WRONG "External users wrong"
#define ENTRY_NOT_INCLOUD_IN_SYMBOLS "Label uses in entry not exist in the symbols list"
#define REGISTRY_OUT_OF_BOUND "Registry does not exist in the system please change to 0-31"
#define I_COMMAND_USE_EXTERNAL "I_Command can't use external symbol!"
#define COMMAND_USE_MISSING_SYMBOL "Command use symbol doesn't exist!"


#endif /* ASMESSAGE_H_ */
