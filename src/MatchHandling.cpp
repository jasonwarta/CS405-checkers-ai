#include "MatchHandling.h"

void Score::assignScore(char result, bool toggle, std::ostream *os)
{
    // (*os) << (toggle ? "1v2" : "2v1") << ": " << result << std::endl;
    switch (result)
    {
    case 'R':
        if (toggle)
            this->p1++;
        else
            this->p2++;
        break;
    case 'B':
        if (toggle)
            this->p2++;
        else
            this->p1++;
        break;
    case 'D':
        this->draw++;
        break;
    default:
        break;
    }
}

void Score::print(std::ostream * os) {
    (*os) << "1:" << int(this->p1) << ", 2:" << int(this->p2) << ", D:" << int(this->draw) << std::endl;
}

bool Score::operator==(const Score & other) const {
    return  this->draw == other.draw &&
            this->p1 == other.p1 &&
            this->p2 == other.p2;
}

void NetTracker::assignScore(Score *gameScore, bool p1)
{
    // gameScore->print();
    std::lock_guard<std::mutex> guard(*mtx_);
    if (p1)
        score += (gameScore->p1 * WIN_VAL) + (gameScore->p2 * LOSS_VAL) + (gameScore->draw * DRAW_VAL);
    else
        score += (gameScore->p2 * WIN_VAL) + (gameScore->p1 * LOSS_VAL) + (gameScore->draw * DRAW_VAL);
}
bool NetTracker::operator == (const NetTracker & other) const {
    return  this->net == other.net &&
            this->score == other.score &&
            this->opponents == other.opponents;
}

std::string playGame(Player *p1, Player *p2, std::string startBoard, Score *score, bool toggle, std::ostream *os)
{
    std::stringstream ss;
    Game game(p1, p2, os);
    score->assignScore(game.run(startBoard), toggle, &ss);
    return ss.str();
}

void Match::playMatch()
{
    std::ofstream ofs;

    std::stringstream ss;
    ofs.open(fname, std::ofstream::out);
    ss << fname << std::endl;

    NeuralNet lhs_net = (*lhs->net);
    NeuralNet rhs_net = (*rhs->net);

    Score score = {0, 0, 0};

    Player p1(true, &lhs_net);
    Player p2(false, &rhs_net);

    ss << playGame(&p1, &p2, startBoard, &score, true, &ofs);
    ss << playGame(&p2, &p1, startBoard, &score, false, &ofs);

    ofs << "\n\n";
    if (score.p1 == score.p2)
    {
        Game game3(&p1, &p2, &ofs);
        score.assignScore(game3.run(), true, &ss);
        ofs << "\n\n";
    }
    ofs.close();

    std::cout << ss.str() << std::endl;

    lhs->assignScore(&score, true);
    rhs->assignScore(&score, false);
}

void Match::operator=(Match & match)
{
    this->lhs = std::move(match.lhs);
    this->rhs = std::move(match.rhs);
    this->startBoard = match.startBoard;
    this->fname = match.fname;
}

void play(std::mutex &mtx, std::queue<std::unique_ptr<Match>> &matches)
{
    while (matches.size() > 0)
    {
        mtx.lock();
        Match m{};
        if (matches.size() > 0)
        {
            m = (*matches.front());
            matches.pop();
        }
        else
        {
            mtx.unlock();
            return;
        }
        mtx.unlock();

        m.playMatch();
    }
}

bool thereExistsAValidMatch(std::vector<size_t> &availableMatches, std::vector<std::shared_ptr<NetTracker>> &nets)
{
    for (size_t i = 0; i < availableMatches.size(); i++)
    {
        for (size_t j = i+1; j < availableMatches.size(); j++)
        {
            if (std::find(nets[availableMatches[i]]->opponents.begin(), nets[availableMatches[i]]->opponents.end(), availableMatches[j]) == nets[availableMatches[i]]->opponents.end())
                return true;
        }
    }
    return false;
}