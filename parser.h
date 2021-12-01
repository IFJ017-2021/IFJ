/**
 *  @file parser.h
 *
 *  @brief Header file for parser.c (Syntax analyzer).
 *
 *  @author <xpoliv06> Tomáš Polívka
 *  @author <xhajek51> Vojtěch Hájek
 */

/**
 * @brief Implementation START rule. <br>
 *  1) require "ifj21" MAIN_LIST <br>
 * @param ... ?
 */
void start();

/**
 * @brief Implementation of MAIN_LIST rule. <br>
 *  1) function function_id (LIST_OF_PARAMS) RETURN_LIST_OF_PARAMS STATEMENT end <br>
 *  2) global function_id : function (LIST_OF_TYPES) RETURN_LIST_OF_TYPES <br>
 *  3) function_id (ENTRY_LIST_PARAMS)
 * @param ... ?
 */
void main_list();

/**
 * @brief Implementation of MAIN_NEXT rule. <br>
 * 1) MAIN_LIST
 * @param ... ?
 */
void main_next();

/**
 * @brief Implementation of STATEMENT rule. <br>
 *  1) value_id VALUE_ID_NEXT = INIT_VALUE INIT_VALUE_NEXT STATEMENT<br>
 *  2) local value_id : TYPE_VALUE INIT_LOCAL_VALUE STATENEBT <br>
 *  3) function_id (ENTRY_LIST_PARAMS) STATEMENT <br>
 *  4) if expression then STATEMENT STATE_ELSE end STATEMENT <br>
 *  5) return RETURN_LIST STATEMENT <br>
 *  6) while expression do STATEMENT end STATEMENT <br>
 * @param ... ?
 */
void statement();

/**
 * @brief Implementation of LIST_OF_PARAMS rule. <br>
 * 1) value_id : TYPE_VALUE PARAM_NEXT <br>
 * @param ... ?
 */
void list_of_params();

/**
 * @brief Implementation of PARAM_NEXT rule. <br>
 * 1) , value_id : TYPE_VALUE PARAM_NEXT <br>
 * @param ... ?
 */
 void param_next();

/**
 * @brief Implementation of ENTRY_LIST_PARAMS rule. <br>
 * 1) ENTRY_PARAM ENTRY_PARAM_NEXT <br>
 * @param ... ?
 */
 void entry_list_params();

/**
 * @brief Implementation of ENTRY_PARAM rule. <br>
 * 1) expression <br>
 * 2) value_id <br>
 * @param ... ?
*/
void entry_param();

/**
 * @brief Implementation of ENTRY_PARAM_NEXT rule. <br>
 * 1) , ENTRY_PARAM ENTRY_PARAM_NEXT <br>
 * @param ... ?
*/
void entry_param_next();

/**
 * @brief Implementation of LIST_OF_TYPES rule. <br>
 * 1) TYPE_VALUE TYPE_NEXT <br>
 * @param ... ?
*/
void list_of_types();

/**
 * @brief Implementation of RETURN_LIST_OF_TYPES  rule. <br>
 * 1) : TYPE_VALUE TYPE_NEXT<br>
 * @param ... ?
*/
void return_list_of_types();

/**
 * @brief Implementation of TYPE_NEXT rule. <br>
 * 1) , TYPE_VALUE TYPE_NEXT <br>
 * @param ... ?
*/
void type_next();

/**
 * @brief Implementation of TYPE_VALUE rule. <br>
 * 1) string <br>
 * 2) number <br>
 * 3) integer <br>
 * 4) nill <br>
 * @param ... ?
*/
void type_value();

/**
 * @brief Implementation of RETURN_LIST rule. <br>
 * 1) function_id(ENTRY_LIST_PARAMS) <br>
 * 2) value_id RETURN_VALUE_NEXT<br>
 * @param ... ?
*/
void return_list();

/**
 * @brief Implementation of RETURN_LIST rule. <br>
 * 1) , value_id RETURN_VALUE_NEXT <br>
 * @param ... ?
*/
void return_value_next();

/**
 * @brief Implementation of STATE_ELSE rule. <br>
 * 1) else STATEMENT <br>
 * @param ... ?
*/
void state_else();

/**
 * @brief Implementation of INIT_VALUE rule. <br>
 * 1) expression <br>
 * 2) function_id(ENTRY_LIST_PARAMS) <br>
 * 3) value_id <br>
 * @param ... ?
*/
void init_value();

/**
 * @brief Implementation of INIT_VALUE_NEXT rule. <br>
 * 1) , INIT_VALUE INIT_VALUE_NEXT <br>
 * @param ... ?
*/
void init_value_next();

/**
 * @brief Implementation of INIT_LOCAL_VALUE rule. <br>
 * 1) = INIT_VALUE <br>
 * @param ... ?
*/
void init_local_value();

/**
 * @brief Implementation of VALUE_ID_NEXT rule. <br>
 * 1) , value_id VALUE_ID_NEXT <br>
 * @param ... ?
*/
void value_id_next();

