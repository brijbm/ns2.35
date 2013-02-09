#ifndef CELL_LINK_HH
#define CELL_LINK_HH

#include<vector>
#include "rng.h"
#include "cdma-rates.h"
#include "delay.h"
#include "flow-stats.h"
#include <map>

/* An implementation of a multiuser Cellular Link.
 * Each user has independent services.
 */

class RateGen {
  public:
    RNG* _rng;
    std::vector<double> _allowed_rates;

    RateGen( std::vector<double> allowed_rates )
    {
      _rng = new RNG();
      _allowed_rates = allowed_rates;
    }
};

class CellLink : public LinkDelay {
  private:
    uint32_t _num_users;
    std::vector<double>  _current_rates;
    std::vector<RateGen> _rate_generators;
    uint32_t _iter;
    uint64_t _bits_dequeued;
    std::map<uint64_t,FlowStats> _flow_stats;
    double _K;
    std::map<uint64_t,double> link_rates_as_map();
    double _next_schedule;
    uint64_t _chosen_flow ;
    static constexpr double SCHEDULING_SLOT_SECS = 1.0;

  public :
    /* Constructor */
    CellLink();

    /* Called by CellLink inside the body of recv() */
    void tick( void );

    /* Generate new rates from allowed rates */
    void generate_new_rates();

    /* Get current rates of all users for SFD::deque() to use.
       SFD could ignore this as well. */
    std::vector<double> get_current_rates() { return _current_rates; }

    /* override the recv function from LinkDelay */
    virtual void recv( Packet* p, Handler* h);

    /* Tcl interface */
    int command(int argc, const char*const* argv);

    /* Link's own prop. fair scheduler */
    uint64_t prop_fair_scheduler();

};

#endif