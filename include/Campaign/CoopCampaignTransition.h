#ifndef COOP_CAMPAIGN_TRANSITION_H
#define COOP_CAMPAIGN_TRANSITION_H

#include <optional>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace coop {

enum class Outcome : unsigned char { Aborted = 0, Lost = 1, Won = 2 };
enum class Advance : unsigned char { Aborted = 0, Retry = 1, Next = 2, Complete = 3 };

// The host accepts only the agreed partner, session and mission. Duplicate
// reports must agree, and a disagreement never advances shared progression.
class MissionBarrier {
public:
    MissionBarrier(std::string session, int stage, std::string partner)
        : MissionBarrier(std::move(session),stage,
              partner.empty() ? std::vector<std::string>{} : std::vector<std::string>{std::move(partner)}) {}

    MissionBarrier(std::string session,int stage,const std::vector<std::string>& partners)
        : session_(std::move(session)),stage_(stage) {
        for(const auto& partner:partners) remote_.emplace(partner,std::nullopt);
    }

    bool receive(const std::string& partner, const std::string& session,
        int stage, Outcome outcome) {
        const auto found=remote_.find(partner);
        if(found==remote_.end() || session != session_ || stage != stage_
            || static_cast<unsigned>(outcome) > static_cast<unsigned>(Outcome::Won)) return false;
        if(found->second && *found->second != outcome) found->second=Outcome::Aborted;
        else found->second=outcome;
        return true;
    }

    std::optional<Advance> decision(Outcome local, int finalStage) const {
        if(local == Outcome::Aborted) return Advance::Aborted;
        for(const auto& report:remote_) {
            if(!report.second) return std::nullopt;
            if(*report.second!=local) return Advance::Aborted;
        }
        if(local == Outcome::Lost) return Advance::Retry;
        return stage_ == finalStage ? Advance::Complete : Advance::Next;
    }

    bool accepts(Advance advance, Outcome local, int finalStage) const {
        if(advance == Advance::Aborted) return true;
        if(local == Outcome::Lost) return advance == Advance::Retry;
        if(local != Outcome::Won) return false;
        return advance == (stage_ == finalStage ? Advance::Complete : Advance::Next);
    }

private:
    std::string session_;
    int stage_;
    std::map<std::string,std::optional<Outcome>> remote_;
};

} // namespace coop
#endif
