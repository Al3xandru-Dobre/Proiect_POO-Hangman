#ifndef SUBJECT_HPP
#define SUBJECT_HPP

#include <vector>
#include <algorithm>

/*
 *Aceasta clasa este un template pentru un subiect
 *Acest design pattern este folosit pentru a notifica mai multi observatori de schimbarile unui obiect
 *Prin metodele de adaugare, stergere si notificare a observerilor, se poate gestiona mai usor evolutia jocului
 *Am ales sa implementez pattern-ul prin template deoarece se poate folosi pentru mai multe tipuri de observatori, putand fi aplicat rapid si usor
 */

template <typename ObserverType>
class Subject {
protected:
    std::vector<ObserverType*> observers; // Lista de observatori

public:
    virtual ~Subject() = default;

    void addObserver(ObserverType* observer) {
        observers.push_back(observer);
    }

    void removeObserver(ObserverType* observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    void notifyObservers() {
        for (ObserverType* observer : observers) {
            observer->update();
        }
    }
};

#endif // SUBJECT_HPP
