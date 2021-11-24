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
 * @return exit code
 */
static int start()

/**
 * @brief Implementation of MAIN_LIST rule. <br>
 *  1) function ID (LIST_OF_PARAMS) RETURN_LIST_OF_PARAMS STATEMENT end <br>
 *  2) global ID : function (LIST_OF_TYPES) RETURN_LIST_OF_TYPES <br>
 *  3) ID (ENTRY_LIST_PARAMS)
 * @param ... ?
 * @return exit code
 */
static int mainList()

/**
 * @brief Implementation of STATEMENT rule. <br>
 *  1)  ID VALUE_ID_NEXT = INIT_VALUE INIT_VALUE_NEXT<br>
 *  2)  local ID = TYPE_VALUE INIT_VALUE <br>
 *  3)  ID (ENTRY_LIST_PARAMS) <br>
 * @param ... ?
 * @return exit code
 */
static int statement()

