/**
 * @file      ac_module.cpp
 * @author    Thiago Massariolli Sigrist
 *
 *            The ArchC Team
 *            http://www.archc.org/
 *
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br/
 *
 * @version   1.0
 * @date      Thu May 12 10:29:52 2005
 * 
 * @brief     Implementation for ArchC abstract module class.
 *
 * @attention Copyright (C) 2002-2006 --- The ArchC Team
 * 
 * This program is free software; you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation; either version 2 of the License, or 
 * (at your option) any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License 
 * along with this program; if not, write to the Free Software 
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

//////////////////////////////////////////////////////////////////////////////

// Standard includes
#include <iostream>
#include <unistd.h>

// SystemC includes

// ArchC includes
#include "ac_module.H"

//////////////////////////////////////////////////////////////////////////////

// 'using' declarations

//////////////////////////////////////////////////////////////////////////////

/// The next module ID.
unsigned ac_module::next_mod_id = 0;

/// Number of currently running modules.
unsigned ac_module::running_mods = 0;

/// List of all modules.
std::list<ac_module*> ac_module::mods_list;

/// Standard constructor.
ac_module::ac_module() : sc_module(sc_gen_unique_name("ac_module")),
			 mod_id(next_mod_id++),
			 ac_exit_status(0){
  ac_qk.set_global_quantum( sc_time(1, SC_NS) );
  ac_qk.reset();
  module_period_ns=5;  //200 MHz = 5ns
  this_mod = mods_list.insert(mods_list.end(), this);
  return;
}

/// Named constructor.
ac_module::ac_module(sc_module_name nm) : sc_module(nm),
			 mod_id(next_mod_id++),
			 ac_exit_status(0){
  ac_qk.set_global_quantum( sc_time(1, SC_NS) );
  ac_qk.reset();
  module_period_ns=5;  //200 MHz = 5ns
  this_mod = mods_list.insert(mods_list.end(), this);
  return;
}

/// Destructor.
ac_module::~ac_module()
{
  mods_list.erase(this_mod);
  return;
}

/// PrintStat placeholder.
void ac_module::PrintStat()
{ // I have no idea whether this class is supposed to be directly instantiated,
  // so I'm providing an implementation. --Marilia
  return;
}

/// Callable PrintStat-like method.
void ac_module::PrintAllStats()
{
  std::list<ac_module*>::iterator i;

  for (i = mods_list.begin(); i != mods_list.end(); i++)
    (*i)->PrintStat();
  return;
}

/// Public method that registers module as a running module.
void ac_module::set_running() {
  running_mods++;
}

/// Public method that unregisters module (ie, it's no longer running).
void ac_module::set_stopped() {
  if (--running_mods == 0) {
    dup2(2, 1); //any output to stdout is redirected for stderr (ex. SystemC stop message)
    sc_stop();
  }
}

/// Public method that sets the size of the uninterrupted instruction batch
void ac_module::set_instr_batch_size(unsigned int size)
{
  //instr_batch_size = size;
}

//Implementation of default sc_time unit SC_NS
void ac_module::set_quantum(unsigned int time_quantum_ns) {
  ac_qk.set_global_quantum( sc_time(time_quantum_ns, SC_NS));
  ac_qk.reset();
}

/// Public method that sets the processor frequency(MHz to ns) 
void ac_module::set_proc_freq(unsigned int proc_freq_mhz) {
  module_period_ns=1000/proc_freq_mhz;
}

