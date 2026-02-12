#include <iostream>
#include <string>
using namespace std;

class Sport {
public:
    Sport(string name) : name_(name) {}
    string name() const {
        return name_;
    }
    virtual bool isOutdoor() const = 0;
    virtual string icon() const = 0;
    virtual ~Sport() {}
private:
    string name_;
    string icon_;
    bool isOutdoor_;
};

class Snowboarding : public Sport {
public:
    Snowboarding(string name) : Sport(name) {}
    bool isOutdoor() const {
        return true;
    }
    string icon() const {
        return "a snowboarder in a tucked grab";
    }
    ~Snowboarding() {
        cout << "Destroying the Snowboarding object named " << name() << "." << endl;
    }
};

class Biathlon : public Sport {
public:
    Biathlon(string name, double dist) : Sport(name), distance_(dist) {}
    bool isOutdoor() const {
        return true;
    }
    string icon() const {
        return "a skier with a rifle";
    }
    ~Biathlon() {
        cout << "Destroying the Biathlon object named " << name()
            << ", distance " << distance_ << " km." << endl;
    }
private:
    double distance_;
};

class FigureSkating : public Sport {
public:
    FigureSkating(string name) : Sport(name) {}
    bool isOutdoor() const {
        return false;
    }
    string icon() const {
        return "a skater in a scratch spin";
    }
    ~FigureSkating() {
        cout << "Destroying the FigureSkating object named " << name() << "." << endl;
    }
};

void display(const Sport* sp)
{
    cout << sp->name() << " is ";
    if (sp->isOutdoor())
        cout << "an outdoor sport ";
    else
        cout << "an indoor sport ";
    cout << "with an icon showing " << sp->icon() << "." << endl;
}

/*int main()
{
    Sport* sports[4];
    sports[0] = new Snowboarding("Women's halfpipe");
    // Biathlon events have a distance in kilometers
    sports[1] = new Biathlon("Men's pursuit", 12.5);
    sports[2] = new Biathlon("Women's sprint", 7.5);
    sports[3] = new FigureSkating("Ice dance");

    cout << "Here are the sports." << endl;
    for (int k = 0; k < 4; k++)
        display(sports[k]);

    // Clean up the sports before exiting
    cout << "Cleaning up." << endl;
    for (int k = 0; k < 4; k++)
        delete sports[k];
}*/