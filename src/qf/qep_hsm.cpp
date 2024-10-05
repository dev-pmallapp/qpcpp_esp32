//$file${src::qf::qep_hsm.cpp} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
// Model: qpcpp.qm
// File:  ${src::qf::qep_hsm.cpp}
//
// This code has been generated by QM 6.2.3 <www.state-machine.com/qm>.
// DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
//
//                 ____________________________________
//                /                                   /
//               /    GGGGGGG    PPPPPPPP   LL       /
//              /   GG     GG   PP     PP  LL       /
//             /   GG          PP     PP  LL       /
//            /   GG   GGGGG  PPPPPPPP   LL       /
//           /   GG      GG  PP         LL       /
//          /     GGGGGGG   PP         LLLLLLL  /
//         /___________________________________/
//
// Copyright (c) 2005 Quantum Leaps, LLC
// SPDX-License-Identifier: GPL-3.0-or-later
//
// This generated code is open-source software licensed under the GNU
// General Public License (GPL) as published by the Free Software Foundation
// (see <https://www.gnu.org/licenses>).
//
// NOTE:
// The GPL does NOT permit the incorporation of this code into proprietary
// programs. Please contact Quantum Leaps for commercial licensing options,
// which expressly supersede the GPL and are designed explicitly for licensees
// interested in retaining the proprietary status of the generated code.
//
// Quantum Leaps contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//
//$endhead${src::qf::qep_hsm.cpp} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#define QP_IMPL             // this is QP implementation
#include "qp_port.hpp"      // QP port
#include "qp_pkg.hpp"       // QP package-scope interface
#include "qsafe.h"          // QP Functional Safety (FuSa) Subsystem
#ifdef Q_SPY                // QS software tracing enabled?
    #include "qs_port.hpp"  // QS port
    #include "qs_pkg.hpp"   // QS facilities for pre-defined trace records
#else
    #include "qs_dummy.hpp" // disable the QS software tracing
#endif // Q_SPY

//============================================================================
//! @cond INTERNAL

// unnamed namespace for local definitions with internal linkage
namespace {
Q_DEFINE_THIS_MODULE("qep_hsm")

// immutable events corresponding to the reserved signals.
static QP::QEvt const l_reservedEvt_[4] {
    QP::QEvt(static_cast<QP::QSignal>(QP::QHsm::Q_EMPTY_SIG)),
    QP::QEvt(static_cast<QP::QSignal>(QP::QHsm::Q_ENTRY_SIG)),
    QP::QEvt(static_cast<QP::QSignal>(QP::QHsm::Q_EXIT_SIG)),
    QP::QEvt(static_cast<QP::QSignal>(QP::QHsm::Q_INIT_SIG))
};

} // unnamed namespace

// helper macro to handle reserved event in an QHsm
#define QHSM_RESERVED_EVT_(state_, sig_) \
    ((*(state_))(this, &l_reservedEvt_[(sig_)]))

// helper macro to trace state entry
#define QS_STATE_ENTRY_(state_, qs_id_)         \
    QS_CRIT_ENTRY();                            \
    QS_MEM_SYS();                               \
    QS_BEGIN_PRE_(QS_QEP_STATE_ENTRY, (qs_id_)) \
        QS_OBJ_PRE_(this);                      \
        QS_FUN_PRE_(state_);                    \
    QS_END_PRE_()                               \
    QS_MEM_APP();                               \
    QS_CRIT_EXIT()

// helper macro to trace state exit
#define QS_STATE_EXIT_(state_, qs_id_)          \
    QS_CRIT_ENTRY();                            \
    QS_MEM_SYS();                               \
    QS_BEGIN_PRE_(QS_QEP_STATE_EXIT, (qs_id_))  \
        QS_OBJ_PRE_(this);                      \
        QS_FUN_PRE_(state_);                    \
    QS_END_PRE_()                               \
    QS_MEM_APP();                               \
    QS_CRIT_EXIT()

//! @endcond
//============================================================================

//$skip${QP_VERSION} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// Check for the minimum required QP version
#if (QP_VERSION < 730U) || (QP_VERSION != ((QP_RELEASE^4294967295U)%0x2710U))
#error qpcpp version 7.3.0 or higher required
#endif
//$endskip${QP_VERSION} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${QEP::versionStr[]} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
namespace QP {

} // namespace QP
//$enddef${QEP::versionStr[]} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//$define${QEP::QHsm} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
namespace QP {

//${QEP::QHsm} ...............................................................

//${QEP::QHsm::QHsm} .........................................................
QHsm::QHsm(QStateHandler const initial) noexcept
: QAsm()
{
    m_state.fun = Q_STATE_CAST(&top);
    m_temp.fun  = initial;
}

//${QEP::QHsm::init} .........................................................
void QHsm::init(
    void const * const e,
    std::uint_fast8_t const qs_id)
{
    QF_CRIT_STAT

    #ifdef Q_SPY
    QS_CRIT_ENTRY();
    QS_MEM_SYS();
    if ((QS::priv_.flags & 0x01U) == 0U) {
        QS::priv_.flags |= 0x01U;
        QS_FUN_DICTIONARY(&QP::QHsm::top);
    }
    QS_MEM_APP();
    QS_CRIT_EXIT();
    #else
    Q_UNUSED_PAR(qs_id);
    #endif

    QStateHandler t = m_state.fun;

    QF_CRIT_ENTRY();
    Q_REQUIRE_INCRIT(200, (m_temp.fun != nullptr)
                      && (t == Q_STATE_CAST(&top)));
    QF_CRIT_EXIT();

    // execute the top-most initial tran.
    QState r = (*m_temp.fun)(this, Q_EVT_CAST(QEvt));

    QF_CRIT_ENTRY();
    // the top-most initial tran. must be taken
    Q_ASSERT_INCRIT(210, r == Q_RET_TRAN);

    QS_MEM_SYS();
    QS_BEGIN_PRE_(QS_QEP_STATE_INIT, qs_id)
        QS_OBJ_PRE_(this);         // this state machine object
        QS_FUN_PRE_(t);            // the source state
        QS_FUN_PRE_(m_temp.fun);   // the target of the initial tran.
    QS_END_PRE_()
    QS_MEM_APP();

    QF_CRIT_EXIT();

    // drill down into the state hierarchy with initial transitions...
    do {
        QStateHandler path[MAX_NEST_DEPTH_]; // tran entry path array
        std::int_fast8_t ip = 0; // entry path index

        path[0] = m_temp.fun;
        static_cast<void>(QHSM_RESERVED_EVT_(m_temp.fun, Q_EMPTY_SIG));
        while (m_temp.fun != t) {
            ++ip;
            QF_CRIT_ENTRY();
            Q_ASSERT_INCRIT(220, ip < MAX_NEST_DEPTH_);
            QF_CRIT_EXIT();
            path[ip] = m_temp.fun;
            static_cast<void>(QHSM_RESERVED_EVT_(m_temp.fun, Q_EMPTY_SIG));
        }
        m_temp.fun = path[0];

        // retrace the entry path in reverse (desired) order...
        do {
            // enter path[ip]
            if (QHSM_RESERVED_EVT_(path[ip], Q_ENTRY_SIG)
                == Q_RET_HANDLED)
            {
                QS_STATE_ENTRY_(path[ip], qs_id);
            }
            --ip;
        } while (ip >= 0);

        t = path[0]; // current state becomes the new source

        r = QHSM_RESERVED_EVT_(t, Q_INIT_SIG); // execute initial tran.

    #ifdef Q_SPY
        if (r == Q_RET_TRAN) {
            QS_CRIT_ENTRY();
            QS_MEM_SYS();
            QS_BEGIN_PRE_(QS_QEP_STATE_INIT, qs_id)
                QS_OBJ_PRE_(this);         // this state machine object
                QS_FUN_PRE_(t);            // the source state
                QS_FUN_PRE_(m_temp.fun);   // the target of the initial tran.
            QS_END_PRE_()
            QS_MEM_APP();
            QS_CRIT_EXIT();
        }
    #endif // Q_SPY

    } while (r == Q_RET_TRAN);

    QS_CRIT_ENTRY();
    QS_MEM_SYS();
    QS_BEGIN_PRE_(QS_QEP_INIT_TRAN, qs_id)
        QS_TIME_PRE_();    // time stamp
        QS_OBJ_PRE_(this); // this state machine object
        QS_FUN_PRE_(t);    // the new active state
    QS_END_PRE_()
    QS_MEM_APP();
    QS_CRIT_EXIT();

    m_state.fun = t;   // change the current active state
    #ifndef Q_UNSAFE
    m_temp.uint = ~m_state.uint;
    #endif
}

//${QEP::QHsm::dispatch} .....................................................
void QHsm::dispatch(
    QEvt const * const e,
    std::uint_fast8_t const qs_id)
{
    #ifndef Q_SPY
    Q_UNUSED_PAR(qs_id);
    #endif

    QStateHandler s = m_state.fun;
    QStateHandler t = s;
    QF_CRIT_STAT

    QF_CRIT_ENTRY();
    Q_REQUIRE_INCRIT(300, (s != Q_STATE_CAST(0))
        && (m_state.uint == static_cast<std::uintptr_t>(~m_temp.uint)));
    Q_REQUIRE_INCRIT(302, QEvt::verify_(e));

    QS_MEM_SYS();
    QS_BEGIN_PRE_(QS_QEP_DISPATCH, qs_id)
        QS_TIME_PRE_();      // time stamp
        QS_SIG_PRE_(e->sig); // the signal of the event
        QS_OBJ_PRE_(this);   // this state machine object
        QS_FUN_PRE_(s);      // the current state
    QS_END_PRE_()
    QS_MEM_APP();

    QF_CRIT_EXIT();

    // process the event hierarchically...
    QState r;
    m_temp.fun = s;
    do {
        s = m_temp.fun;
        r = (*s)(this, e); // invoke state handler s

        if (r == Q_RET_UNHANDLED) { // unhandled due to a guard?

            QS_CRIT_ENTRY();
            QS_MEM_SYS();
            QS_BEGIN_PRE_(QS_QEP_UNHANDLED, qs_id)
                QS_SIG_PRE_(e->sig); // the signal of the event
                QS_OBJ_PRE_(this);   // this state machine object
                QS_FUN_PRE_(s);      // the current state
            QS_END_PRE_()
            QS_MEM_APP();
            QS_CRIT_EXIT();

            r = QHSM_RESERVED_EVT_(s, Q_EMPTY_SIG); // superstate of s
        }
    } while (r == Q_RET_SUPER);

    if (r >= Q_RET_TRAN) { // regular tran. taken?
        QStateHandler path[MAX_NEST_DEPTH_];

        path[0] = m_temp.fun; // tran. target
        path[1] = t; // current state
        path[2] = s; // tran. source

        // exit current state to tran. source s...
        for (; t != s; t = m_temp.fun) {
            // exit from t
            if (QHSM_RESERVED_EVT_(t, Q_EXIT_SIG) == Q_RET_HANDLED) {
                QS_STATE_EXIT_(t, qs_id);
                // find superstate of t
                static_cast<void>(QHSM_RESERVED_EVT_(t, Q_EMPTY_SIG));
            }
        }
        std::int_fast8_t ip = hsm_tran(path, qs_id); // take the tran.

    #ifdef Q_SPY
        if (r == Q_RET_TRAN_HIST) {
            QS_CRIT_ENTRY();
            QS_MEM_SYS();
            QS_BEGIN_PRE_(QS_QEP_TRAN_HIST, qs_id)
                QS_OBJ_PRE_(this);    // this state machine object
                QS_FUN_PRE_(t);       // the source of the transition
                QS_FUN_PRE_(path[0]); // the target of the tran. to history
            QS_END_PRE_()
            QS_MEM_APP();
            QS_CRIT_EXIT();
        }
    #endif // Q_SPY

        // execute state entry actions in the desired order...
        for (; ip >= 0; --ip) {
            // enter path[ip]
            if (QHSM_RESERVED_EVT_(path[ip], Q_ENTRY_SIG)
                == Q_RET_HANDLED)
            {
                QS_STATE_ENTRY_(path[ip], qs_id);
            }
        }
        t = path[0];    // stick the target into register
        m_temp.fun = t; // update the next state

        // drill into the target hierarchy...
        while (QHSM_RESERVED_EVT_(t, Q_INIT_SIG) == Q_RET_TRAN) {

            QS_CRIT_ENTRY();
            QS_MEM_SYS();
            QS_BEGIN_PRE_(QS_QEP_STATE_INIT, qs_id)
                QS_OBJ_PRE_(this);         // this state machine object
                QS_FUN_PRE_(t);            // the source (pseudo)state
                QS_FUN_PRE_(m_temp.fun);   // the target of the tran.
            QS_END_PRE_()
            QS_MEM_APP();
            QS_CRIT_EXIT();

            ip = 0;
            path[0] = m_temp.fun;

            // find superstate
            static_cast<void>(QHSM_RESERVED_EVT_(m_temp.fun, Q_EMPTY_SIG));

            while (m_temp.fun != t) {
                ++ip;
                path[ip] = m_temp.fun;
                // find superstate
                static_cast<void>(
                    QHSM_RESERVED_EVT_(m_temp.fun, Q_EMPTY_SIG));
            }
            m_temp.fun = path[0];

            // entry path must not overflow
            QF_CRIT_ENTRY();
            Q_ASSERT_INCRIT(410, ip < MAX_NEST_DEPTH_);
            QF_CRIT_EXIT();

            // retrace the entry path in reverse (correct) order...
            do {
                // enter path[ip]
                if (QHSM_RESERVED_EVT_(path[ip], Q_ENTRY_SIG)
                    == Q_RET_HANDLED)
                {
                    QS_STATE_ENTRY_(path[ip], qs_id);
                }
                --ip;
            } while (ip >= 0);

            t = path[0]; // current state becomes the new source
        }

        QS_CRIT_ENTRY();
        QS_MEM_SYS();
        QS_BEGIN_PRE_(QS_QEP_TRAN, qs_id)
            QS_TIME_PRE_();      // time stamp
            QS_SIG_PRE_(e->sig); // the signal of the event
            QS_OBJ_PRE_(this);   // this state machine object
            QS_FUN_PRE_(s);      // the source of the tran.
            QS_FUN_PRE_(t);      // the new active state
        QS_END_PRE_()
        QS_MEM_APP();
        QS_CRIT_EXIT();
    }

    #ifdef Q_SPY
    else if (r == Q_RET_HANDLED) {
        QS_CRIT_ENTRY();
        QS_MEM_SYS();
        QS_BEGIN_PRE_(QS_QEP_INTERN_TRAN, qs_id)
            QS_TIME_PRE_();      // time stamp
            QS_SIG_PRE_(e->sig); // the signal of the event
            QS_OBJ_PRE_(this);   // this state machine object
            QS_FUN_PRE_(s);      // the source state
        QS_END_PRE_()
        QS_MEM_APP();
        QS_CRIT_EXIT();
    }
    else {
        QS_CRIT_ENTRY();
        QS_MEM_SYS();
        QS_BEGIN_PRE_(QS_QEP_IGNORED, qs_id)
            QS_TIME_PRE_();      // time stamp
            QS_SIG_PRE_(e->sig); // the signal of the event
            QS_OBJ_PRE_(this);   // this state machine object
            QS_FUN_PRE_(m_state.fun); // the current state
        QS_END_PRE_()
        QS_MEM_APP();
        QS_CRIT_EXIT();
    }
    #endif // Q_SPY

    m_state.fun = t; // change the current active state
    #ifndef Q_UNSAFE
    m_temp.uint = ~m_state.uint;
    #endif
}

//${QEP::QHsm::isIn} .........................................................
bool QHsm::isIn(QStateHandler const state) noexcept {
    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    Q_REQUIRE_INCRIT(602, m_state.uint
                      == static_cast<std::uintptr_t>(~m_temp.uint));
    QF_CRIT_EXIT();

    bool inState = false; // assume that this HSM is not in 'state'

    // scan the state hierarchy bottom-up
    QState r;
    do {
        // do the states match?
        if (m_temp.fun == state) {
            inState = true;    // 'true' means that match found
            r = Q_RET_IGNORED; // cause breaking out of the loop
        }
        else {
            r = QHSM_RESERVED_EVT_(m_temp.fun, Q_EMPTY_SIG);
        }
    } while (r != Q_RET_IGNORED); // QHsm::top() state not reached

    #ifndef Q_UNSAFE
    m_temp.uint = ~m_state.uint;
    #endif

    return inState; // return the status
}

//${QEP::QHsm::childState} ...................................................
QStateHandler QHsm::childState(QStateHandler const parent) noexcept {
    QStateHandler child = m_state.fun; // start with the current state
    bool isFound = false; // start with the child not found

    // establish stable state configuration
    m_temp.fun = child;
    QState r;
    do {
        // is this the parent of the current child?
        if (m_temp.fun == parent) {
            isFound = true; // child is found
            r = Q_RET_IGNORED; // break out of the loop
        }
        else {
            child = m_temp.fun;
            r = QHSM_RESERVED_EVT_(m_temp.fun, Q_EMPTY_SIG);
        }
    } while (r != Q_RET_IGNORED); // QHsm::top() state not reached

    #ifndef Q_UNSAFE
    m_temp.uint = ~m_state.uint;
    #endif

    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    Q_ASSERT_INCRIT(890, isFound);
    QF_CRIT_EXIT();

    return child; // return the child
}

//${QEP::QHsm::hsm_tran} .....................................................
std::int_fast8_t QHsm::hsm_tran(
    QStateHandler (&path)[MAX_NEST_DEPTH_],
    std::uint_fast8_t const qs_id)
{
    #ifndef Q_SPY
    Q_UNUSED_PAR(qs_id);
    #endif

    std::int_fast8_t ip = -1; // tran. entry path index
    QStateHandler t = path[0];
    QStateHandler const s = path[2];
    QF_CRIT_STAT

    // (a) check source==target (tran. to self)...
    if (s == t) {
        // exit source s
        if (QHSM_RESERVED_EVT_(s, Q_EXIT_SIG) == Q_RET_HANDLED) {
            QS_STATE_EXIT_(s, qs_id);
        }
        ip = 0; // enter the target
    }
    else {
        // find superstate of target
        static_cast<void>(QHSM_RESERVED_EVT_(t, Q_EMPTY_SIG));

        t = m_temp.fun;

        // (b) check source==target->super...
        if (s == t) {
            ip = 0; // enter the target
        }
        else {
            // find superstate of src
            static_cast<void>(QHSM_RESERVED_EVT_(s, Q_EMPTY_SIG));

            // (c) check source->super==target->super...
            if (m_temp.fun == t) {
                // exit source s
                if (QHSM_RESERVED_EVT_(s, Q_EXIT_SIG) == Q_RET_HANDLED) {
                    QS_STATE_EXIT_(s, qs_id);
                }
                ip = 0; // enter the target
            }
            else {
                // (d) check source->super==target...
                if (m_temp.fun == path[0]) {
                    // exit source s
                    if (QHSM_RESERVED_EVT_(s, Q_EXIT_SIG) == Q_RET_HANDLED) {
                        QS_STATE_EXIT_(s, qs_id);
                    }
                }
                else {
                    // (e) check rest of source==target->super->super..
                    // and store the entry path along the way
                    std::int_fast8_t iq = 0; // indicate that LCA was found
                    ip = 1; // enter target and its superstate
                    path[1] = t;    // save the superstate of target
                    t = m_temp.fun; // save source->super

                    // find target->super->super...
                    QState r = QHSM_RESERVED_EVT_(path[1], Q_EMPTY_SIG);
                    while (r == Q_RET_SUPER) {
                        ++ip;
                        path[ip] = m_temp.fun; // store the entry path
                        if (m_temp.fun == s) { // is it the source?
                            iq = 1; // indicate that the LCA found

                            // entry path must not overflow
                            QF_CRIT_ENTRY();
                            Q_ASSERT_INCRIT(510, ip < MAX_NEST_DEPTH_);
                            QF_CRIT_EXIT();
                            --ip; // do not enter the source
                            r = Q_RET_HANDLED; // terminate the loop
                        }
                        else { // it is not the source, keep going up
                            r = QHSM_RESERVED_EVT_(m_temp.fun, Q_EMPTY_SIG);
                        }
                    }

                    // the LCA not found yet?
                    if (iq == 0) {
                        // entry path must not overflow
                        QF_CRIT_ENTRY();
                        Q_ASSERT_INCRIT(520, ip < MAX_NEST_DEPTH_);
                        QF_CRIT_EXIT();

                        // exit source s
                        if (QHSM_RESERVED_EVT_(s, Q_EXIT_SIG)
                            == Q_RET_HANDLED)
                        {
                            QS_STATE_EXIT_(s, qs_id);
                        }

                        // (f) check the rest of source->super
                        //                  == target->super->super...
                        iq = ip;
                        r = Q_RET_IGNORED; // indicate that the LCA NOT found
                        do {
                            if (t == path[iq]) { // is this the LCA?
                                r = Q_RET_HANDLED; // indicate the LCA found
                                ip = iq - 1; // do not enter the LCA
                                iq = -1; // cause termination of the loop
                            }
                            else {
                                --iq; // try lower superstate of target
                            }
                        } while (iq >= 0);

                        // the LCA not found yet?
                        if (r != Q_RET_HANDLED) {
                            // (g) check each source->super->...
                            // for each target->super...
                            r = Q_RET_IGNORED; // keep looping
                            do {
                                // exit from t
                                if (QHSM_RESERVED_EVT_(t, Q_EXIT_SIG)
                                    == Q_RET_HANDLED)
                                {
                                    QS_STATE_EXIT_(t, qs_id);
                                    // find superstate of t
                                    static_cast<void>(
                                        QHSM_RESERVED_EVT_(t, Q_EMPTY_SIG));
                                }
                                t = m_temp.fun; // set to super of t
                                iq = ip;
                                do {
                                    // is this the LCA?
                                    if (t == path[iq]) {
                                        ip = iq - 1; // do not enter the LCA
                                        iq = -1;     // break out of inner loop
                                        r = Q_RET_HANDLED; // break outer loop
                                    }
                                    else {
                                        --iq;
                                    }
                                } while (iq >= 0);
                            } while (r != Q_RET_HANDLED);
                        }
                    }
                }
            }
        }
    }
    return ip;
}

} // namespace QP
//$enddef${QEP::QHsm} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
