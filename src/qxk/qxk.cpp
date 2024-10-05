//$file${src::qxk::qxk.cpp} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
// Model: qpcpp.qm
// File:  ${src::qxk::qxk.cpp}
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
//$endhead${src::qxk::qxk.cpp} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
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

// protection against including this source file in a wrong project
#ifndef QXK_HPP_
    #error "Source file included in a project NOT based on the QXK kernel"
#endif // QXK_HPP_

// unnamed namespace for local definitions with internal linkage
namespace {
Q_DEFINE_THIS_MODULE("qxk")
} // unnamed namespace

//$skip${QP_VERSION} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// Check for the minimum required QP version
#if (QP_VERSION < 730U) || (QP_VERSION != ((QP_RELEASE^4294967295U)%0x2710U))
#error qpcpp version 7.3.0 or higher required
#endif
//$endskip${QP_VERSION} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${QXK::QXK-base} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
namespace QP {
namespace QXK {

//${QXK::QXK-base::schedLock} ................................................
QSchedStatus schedLock(std::uint_fast8_t const ceiling) noexcept {
    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();

    Q_REQUIRE_INCRIT(100, !QXK_ISR_CONTEXT_());

    QSchedStatus stat; // saved lock status to be returned

    // is the lock ceiling being raised?
    if (ceiling > QXK_priv_.lockCeil) {
        QS_BEGIN_PRE_(QS_SCHED_LOCK, 0U)
            QS_TIME_PRE_(); // timestamp
            // the previous lock ceiling & new lock ceiling
            QS_2U8_PRE_(static_cast<std::uint8_t>(QXK_priv_.lockCeil),
                        static_cast<std::uint8_t>(ceiling));
        QS_END_PRE_()

        // previous status of the lock
        stat  = static_cast<QSchedStatus>(QXK_priv_.lockHolder);
        stat |= static_cast<QSchedStatus>(QXK_priv_.lockCeil) << 8U;

        // new status of the lock
        QXK_priv_.lockHolder = (QXK_priv_.curr != nullptr)
                               ? QXK_priv_.curr->getPrio()
                               : 0U;
        QXK_priv_.lockCeil   = ceiling;
    }
    else {
       stat = 0xFFU; // scheduler not locked
    }
    QF_MEM_APP();
    QF_CRIT_EXIT();

    return stat; // return the status to be saved in a stack variable
}

//${QXK::QXK-base::schedUnlock} ..............................................
void schedUnlock(QSchedStatus const stat) noexcept {
    // has the scheduler been actually locked by the last QXK::schedLock()?
    if (stat != 0xFFU) {
        std::uint8_t const prevCeil = static_cast<std::uint8_t>(stat >> 8U);
        QF_CRIT_STAT
        QF_CRIT_ENTRY();
        QF_MEM_SYS();

        Q_REQUIRE_INCRIT(200, !QXK_ISR_CONTEXT_());
        Q_REQUIRE_INCRIT(201, QXK_priv_.lockCeil > prevCeil);

        QS_BEGIN_PRE_(QS_SCHED_UNLOCK, 0U)
            QS_TIME_PRE_(); // timestamp
            // ceiling before unlocking & prio after unlocking
            QS_2U8_PRE_(QXK_priv_.lockCeil, prevCeil);
        QS_END_PRE_()

        // restore the previous lock ceiling and lock holder
        QXK_priv_.lockCeil   = prevCeil;
        QXK_priv_.lockHolder = (stat & 0xFFU);

        // find if any threads should be run after unlocking the scheduler
        if (QXK_sched_() != 0U) { // activation needed?
            QXK_activate_(); // synchronously activate basic-thred(s)
        }

        QF_MEM_APP();
        QF_CRIT_EXIT();
    }
}

//${QXK::QXK-base::current} ..................................................
QP::QActive * current() noexcept {
    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();

    Q_REQUIRE_INCRIT(600, QXK_priv_.lockCeil <= QF_MAX_ACTIVE);

    QP::QActive *curr = QXK_priv_.curr;
    if (curr == nullptr) { // basic thread?
        curr = QP::QActive::registry_[QXK_priv_.actPrio];
    }

    Q_ASSERT_INCRIT(690, curr != nullptr);

    QF_MEM_APP();
    QF_CRIT_EXIT();

    return curr;
}

} // namespace QXK
} // namespace QP
//$enddef${QXK::QXK-base} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

extern "C" {
//$define${QXK-extern-C} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${QXK-extern-C::QXK_priv_} .................................................
QXK_Attr QXK_priv_;

//${QXK-extern-C::QXK_sched_} ................................................
std::uint_fast8_t QXK_sched_() noexcept {
    Q_REQUIRE_INCRIT(402,
        QXK_priv_.readySet.verify_(&QXK_priv_.readySet_dis));

    std::uint_fast8_t p;
    if (QXK_priv_.readySet.isEmpty()) {
        p = 0U; // no activation needed
    }
    else {
        // find the highest-prio thread ready to run
        p = QXK_priv_.readySet.findMax();
        if (p <= QXK_priv_.lockCeil) {
            // prio. of the thread holding the lock
            p = static_cast<std::uint_fast8_t>(
                 QP::QActive::registry_[QXK_priv_.lockHolder]->getPrio());
            if (p != 0U) {
                Q_ASSERT_INCRIT(410, QXK_priv_.readySet.hasElement(p));
            }
        }
    }
    QP::QActive const * const curr = QXK_priv_.curr;
    QP::QActive * const next = QP::QActive::registry_[p];

    // the next thread found must be registered in QF
    Q_ASSERT_INCRIT(420, next != nullptr);

    // is the current thread a basic-thread?
    if (curr == nullptr) {

        // is the new prio. above the active prio.?
        if (p > QXK_priv_.actPrio) {
            QXK_priv_.next = next; // set the next AO to activate

            if (next->getOsObject() != nullptr) { // is next extended?
                QXK_CONTEXT_SWITCH_();
                p = 0U; // no activation needed
            }
        }
        else { // below the pre-thre
            QXK_priv_.next = nullptr;
            p = 0U; // no activation needed
        }
    }
    else { // currently executing an extended-thread
        // is the current thread different from the next?
        if (curr != next) {
            QXK_priv_.next = next;
            QXK_CONTEXT_SWITCH_();
        }
        else { // current is the same as next
            QXK_priv_.next = nullptr; // no need to context-switch
        }
        p = 0U; // no activation needed
    }

    return p;
}

//${QXK-extern-C::QXK_activate_} .............................................
void QXK_activate_() noexcept {
    std::uint_fast8_t const prio_in = QXK_priv_.actPrio;
    QP::QActive *next = QXK_priv_.next; // the next AO (basic-thread) to run

    Q_REQUIRE_INCRIT(500, (next != nullptr) && (prio_in <= QF_MAX_ACTIVE));

    // QXK Context switch callback defined or QS tracing enabled?
    #if (defined QF_ON_CONTEXT_SW) || (defined Q_SPY)
    QXK_contextSw_(next);
    #endif // QF_ON_CONTEXT_SW || Q_SPY

    QXK_priv_.next = nullptr; // clear the next AO
    QXK_priv_.curr = nullptr; // current is basic-thread

    // prio. of the next thread
    std::uint_fast8_t p = next->getPrio();

    // loop until no more ready-to-run AOs of higher prio than the initial
    do  {
        QXK_priv_.actPrio = p; // next active prio

        QF_INT_ENABLE(); // unconditionally enable interrupts

        QP::QEvt const * const e = next->get_();
        // NOTE QActive::get_() performs QS_MEM_APP() before return

        // dispatch event (virtual call)
        next->dispatch(e, next->getPrio());
    #if (QF_MAX_EPOOL > 0U)
        QP::QF::gc(e);
    #endif

        QF_INT_DISABLE(); // unconditionally disable interrupts
        QF_MEM_SYS();

        // check internal integrity (duplicate inverse storage)
        Q_ASSERT_INCRIT(502,
            QXK_priv_.readySet.verify_(&QXK_priv_.readySet_dis));

        if (next->getEQueue().isEmpty()) { // empty queue?
            QXK_priv_.readySet.remove(p);
    #ifndef Q_UNSAFE
            QXK_priv_.readySet.update_(&QXK_priv_.readySet_dis);
    #endif
        }

        if (QXK_priv_.readySet.isEmpty()) {
            QXK_priv_.next = nullptr;
            next = QP::QActive::registry_[0];
            p = 0U; // no activation needed
        }
        else {
            // find next highest-prio below the lock ceiling
            p = QXK_priv_.readySet.findMax();
            if (p <= QXK_priv_.lockCeil) {
                p = QXK_priv_.lockHolder;
                if (p != 0U) {
                    Q_ASSERT_INCRIT(510, QXK_priv_.readySet.hasElement(p));
                }
            }

            // set the next thread and ensure that it is registered
            next = QP::QActive::registry_[p];
            Q_ASSERT_INCRIT(520, next != nullptr);

            // is next a basic thread?
            if (next->getOsObject() == nullptr) {
                // is the next prio. above the initial prio.?
                if (p > QP::QActive::registry_[prio_in]->getPrio()) {
    #if (defined QF_ON_CONTEXT_SW) || (defined Q_SPY)
                    if (p != QXK_priv_.actPrio) { // changing threads?
                        QXK_contextSw_(next);
                    }
    #endif // QF_ON_CONTEXT_SW || Q_SPY
                    QXK_priv_.next = next;
                }
                else {
                    QXK_priv_.next = nullptr;
                    p = 0U; // no activation needed
                }
            }
            else {  // next is the extended-thread
                QXK_priv_.next = next;
                QXK_CONTEXT_SWITCH_();
                p = 0U; // no activation needed
            }
        }
    } while (p != 0U); // while activation needed

    // restore the active prio.
    QXK_priv_.actPrio = prio_in;

    #if (defined QF_ON_CONTEXT_SW) || (defined Q_SPY)
    if (next->getOsObject() == nullptr) {
        QXK_contextSw_((prio_in == 0U)
                       ? nullptr
                       : QP::QActive::registry_[prio_in]);
    }
    #endif // QF_ON_CONTEXT_SW || Q_SPY
}

//${QXK-extern-C::QXK_contextSw_} ............................................
void QXK_contextSw_(QP::QActive * const next) {
    #ifdef Q_SPY
    std::uint_fast8_t const prev_prio = (QXK_priv_.prev != nullptr)
                             ? QXK_priv_.prev->getPrio()
                             : 0U;
    if (next != nullptr) { // next is NOT idle?
        std::uint_fast8_t const next_prio = next->getPrio();
        QS_BEGIN_PRE_(QP::QS_SCHED_NEXT, next_prio)
            QS_TIME_PRE_(); // timestamp
            QS_2U8_PRE_(next_prio, prev_prio);
        QS_END_PRE_()
    }
    else { // going to idle
        QS_BEGIN_PRE_(QP::QS_SCHED_IDLE, prev_prio)
            QS_TIME_PRE_(); // timestamp
            QS_U8_PRE_(prev_prio);
        QS_END_PRE_()
    }
    #endif // Q_SPY

    #ifdef QF_ON_CONTEXT_SW
    QF_onContextSw(QXK_priv_.prev, next);
    #endif // QF_ON_CONTEXT_SW

    QXK_priv_.prev = next; // update the previous thread
}

//${QXK-extern-C::QXK_threadExit_} ...........................................
void QXK_threadExit_() {
    QF_CRIT_STAT
    QF_CRIT_ENTRY();

    QP::QXThread const * const thr = QXTHREAD_CAST_(QXK_priv_.curr);

    Q_REQUIRE_INCRIT(900, (!QXK_ISR_CONTEXT_())
        && (thr != nullptr)); // current thread must be extended
    Q_REQUIRE_INCRIT(901, QXK_priv_.lockHolder != thr->getPrio());

    std::uint_fast8_t const p =
        static_cast<std::uint_fast8_t>(thr->getPrio());

    QF_MEM_SYS();
    QP::QActive::registry_[p] = nullptr;
    QXK_priv_.readySet.remove(p);
    #ifndef Q_UNSAFE
    QXK_priv_.readySet.update_(&QXK_priv_.readySet_dis);
    #endif

    static_cast<void>(QXK_sched_()); // schedule other threads

    QF_MEM_APP();
    QF_CRIT_EXIT();
}
//$enddef${QXK-extern-C} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
} // extern "C"

//$define${QXK::QF-cust} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
namespace QP {
namespace QF {

//${QXK::QF-cust::init} ......................................................
void init() {
    bzero_(&QF::priv_,                 sizeof(QF::priv_));
    bzero_(&QXK_priv_,                 sizeof(QXK_priv_));
    bzero_(&QTimeEvt::timeEvtHead_[0], sizeof(QTimeEvt::timeEvtHead_));
    bzero_(&QActive::registry_[0],     sizeof(QActive::registry_));

    #ifndef Q_UNSAFE
    QXK_priv_.readySet.update_(&QXK_priv_.readySet_dis);
    #endif

    // setup the QXK scheduler as initially locked and not running
    QXK_priv_.lockCeil = (QF_MAX_ACTIVE + 1U); // scheduler locked

    // storage capable for holding a blank QActive object (const in ROM)
    static void* const
        idle_ao[((sizeof(QActive) + sizeof(void*)) - 1U) / sizeof(void*)]
            = { nullptr };

    // register the idle AO object (cast 'const' away)
    QActive::registry_[0] = QF_CONST_CAST_(QActive*,
        reinterpret_cast<QActive const*>(idle_ao));

    #ifdef QXK_INIT
    QXK_INIT(); // port-specific initialization of the QXK kernel
    #endif
}

//${QXK::QF-cust::stop} ......................................................
void stop() {
    onCleanup(); // cleanup callback
    // nothing else to do for the QXK preemptive kernel
}

//${QXK::QF-cust::run} .......................................................
int_t run() {
    #ifdef Q_SPY
    QS_SIG_DICTIONARY(QXK::DELAY_SIG,   nullptr);
    QS_SIG_DICTIONARY(QXK::TIMEOUT_SIG, nullptr);

    // produce the QS_QF_RUN trace record
    QF_INT_DISABLE();
    QF_MEM_SYS();
    QS::beginRec_(QS_REC_NUM_(QS_QF_RUN));
    QS::endRec_();
    QF_MEM_APP();
    QF_INT_ENABLE();
    #endif // Q_SPY

    onStartup(); // application-specific startup callback

    QF_INT_DISABLE();
    QF_MEM_SYS();

    QXK_priv_.lockCeil = 0U; // unlock the QXK scheduler

    // activate AOs to process events posted so far
    if (QXK_sched_() != 0U) {
        QXK_activate_();
    }

    #ifdef QXK_START
    QXK_START(); // port-specific startup of the QXK kernel
    #endif

    QF_MEM_APP();
    QF_INT_ENABLE();

    for (;;) { // QXK idle loop...
        QXK::onIdle(); // application-specific QXK idle callback
    }

    #ifdef __GNUC__  // GNU compiler?
    return 0;
    #endif
}

} // namespace QF
} // namespace QP
//$enddef${QXK::QF-cust} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//$define${QXK::QActive} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
namespace QP {

//${QXK::QActive} ............................................................

//${QXK::QActive::start} .....................................................
void QActive::start(
    QPrioSpec const prioSpec,
    QEvt const * * const qSto,
    std::uint_fast16_t const qLen,
    void * const stkSto,
    std::uint_fast16_t const stkSize,
    void const * const par)
{
    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    Q_REQUIRE_INCRIT(300, (!QXK_ISR_CONTEXT_())
                     && ((prioSpec & 0xFF00U) == 0U));
    QF_CRIT_EXIT();

    m_prio  = static_cast<std::uint8_t>(prioSpec & 0xFFU); // QF-prio.
    m_pthre = 0U; // preemption-threshold NOT used
    register_(); // make QF aware of this QActive/QXThread

    if (stkSto == nullptr) { // starting basic thread (AO)?
        m_eQueue.init(qSto, qLen); // init the built-in queue
        m_osObject  = nullptr; // no private stack for AO

        this->init(par, m_prio); // top-most initial tran. (virtual call)
        QS_FLUSH(); // flush the trace buffer to the host

        // see if this AO needs to be scheduled if QXK is already running
        QF_CRIT_ENTRY();
        QF_MEM_SYS();
        if (QXK_priv_.lockCeil <= QF_MAX_ACTIVE) { // scheduler running?
            if (QXK_sched_() != 0U) { // activation needed?
                QXK_activate_(); // synchronously activate basic-thred(s)
            }
        }
        QF_MEM_APP();
        QF_CRIT_EXIT();
    }
    else { // starting QXThread

        // is storage for the queue buffer provided?
        if (qSto != nullptr) {
            m_eQueue.init(qSto, qLen);
        }

        // extended threads provide their thread function in place of
        // the top-most initial tran. 'm_temp.act'
        QXK_PTR_CAST_(QXThread*, this)->stackInit_(m_temp.thr,
                                                   stkSto, stkSize);

        // the new thread is not blocked on any object
        m_temp.obj = nullptr;

        QF_CRIT_ENTRY();
        QF_MEM_SYS();

        // extended-thread becomes ready immediately
        QXK_priv_.readySet.insert(static_cast<std::uint_fast8_t>(m_prio));
        #ifndef Q_UNSAFE
        QXK_priv_.readySet.update_(&QXK_priv_.readySet_dis);
        #endif

        // see if this thread needs to be scheduled in case QXK is running
        if (QXK_priv_.lockCeil <= QF_MAX_ACTIVE) {
            static_cast<void>(QXK_sched_()); // schedule other threads
        }
        QF_MEM_APP();
        QF_CRIT_EXIT();
    }
}

} // namespace QP
//$enddef${QXK::QActive} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
