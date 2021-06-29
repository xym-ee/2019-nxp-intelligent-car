/*  <The 14th National University Students Intelligent Car Race.>
 *  Copyright (C) <2019>  < github.com:He0L1w  NJUST >
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef __UI_H
#define __UI_H

#define list_entry(ptr, type, number)    \
                    ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->number)))
                      
                      

typedef struct _ui_selection ui_selection_t;

struct _ui_selection
{
  char              txt[16];        /* OLED一行显示的内容 */
  void              *data_addr;     /* OLED一行中要修改数据地址 */
  uint8_t           byte;           /* 数据字节数 */
  ui_selection_t    *next;
};








void UI_debugsetting(void);





#endif




