#include "Algorithms.h"

#include <algorithm>
#include <cstddef>
#include <utility>
#include <vector>

namespace {

bool compareByFinish(const Course& a, const Course& b) {
    if (a.finish != b.finish) {
        return a.finish < b.finish;
    }
    if (a.start != b.start) {
        return a.start < b.start;
    }
    return a.id < b.id;
}

bool compareByStart(const Course& a, const Course& b) {
    if (a.start != b.start) {
        return a.start < b.start;
    }
    if (a.finish != b.finish) {
        return a.finish < b.finish;
    }
    return a.id < b.id;
}

using CourseComparator = bool (*)(const Course&, const Course&);

void mergeRanges(std::vector<Course>& courses,
                 std::size_t left,
                 std::size_t mid,
                 std::size_t right,
                 CourseComparator comparator) {
    std::vector<Course> merged;
    merged.reserve(right - left);

    std::size_t i = left;
    std::size_t j = mid;
    while (i < mid && j < right) {
        if (!comparator(courses[j], courses[i])) {
            merged.push_back(std::move(courses[i]));
            ++i;
        } else {
            merged.push_back(std::move(courses[j]));
            ++j;
        }
    }
    while (i < mid) {
        merged.push_back(std::move(courses[i]));
        ++i;
    }
    while (j < right) {
        merged.push_back(std::move(courses[j]));
        ++j;
    }
    for (std::size_t k = 0; k < merged.size(); ++k) {
        courses[left + k] = std::move(merged[k]);
    }
}

void mergeSortRecursive(std::vector<Course>& courses,
                        std::size_t left,
                        std::size_t right,
                        CourseComparator comparator) {
    if (right - left <= 1) {
        return;
    }
    std::size_t mid = left + (right - left) / 2;
    mergeSortRecursive(courses, left, mid, comparator);
    mergeSortRecursive(courses, mid, right, comparator);
    mergeRanges(courses, left, mid, right, comparator);
}

void mergeSortWithComparator(std::vector<Course>& courses,
                             CourseComparator comparator) {
    if (courses.size() <= 1) {
        return;
    }
    mergeSortRecursive(courses, 0, courses.size(), comparator);
}

bool coursesOverlap(const Course& a, const Course& b) {
    return !(a.finish <= b.start || b.finish <= a.start);
}

bool isSubsetValid(const std::vector<Course>& courses, std::size_t mask) {
    for (std::size_t i = 0; i < courses.size(); ++i) {
        if ((mask & (std::size_t(1) << i)) == 0) {
            continue;
        }
        for (std::size_t j = i + 1; j < courses.size(); ++j) {
            if ((mask & (std::size_t(1) << j)) != 0 &&
                coursesOverlap(courses[i], courses[j])) {
                return false;
            }
        }
    }
    return true;
}

void searchMinClassrooms(const std::vector<Course>& courses,
                         std::size_t index,
                         std::vector<std::vector<Course>>& rooms,
                         std::size_t& bestRooms) {
    if (rooms.size() >= bestRooms) {
        return;
    }
    if (index == courses.size()) {
        bestRooms = rooms.size();
        return;
    }

    const Course& current = courses[index];
    for (std::size_t room = 0; room < rooms.size(); ++room) {
        bool fits = true;
        for (const Course& scheduled : rooms[room]) {
            if (coursesOverlap(current, scheduled)) {
                fits = false;
                break;
            }
        }
        if (fits) {
            rooms[room].push_back(current);
            searchMinClassrooms(courses, index + 1, rooms, bestRooms);
            rooms[room].pop_back();
        }
    }

    if (rooms.size() + 1 < bestRooms) {
        rooms.push_back({current});
        searchMinClassrooms(courses, index + 1, rooms, bestRooms);
        rooms.pop_back();
    }
}

} // namespace

void mergeSort(std::vector<Course>& courses) {
    mergeSortWithComparator(courses, compareByFinish);
}

std::vector<Course> selectCourses(
    const std::vector<Course>& sortedCourses) {
    std::vector<Course> selected;
    if (sortedCourses.empty()) {
        return selected;
    }

    selected.push_back(sortedCourses[0]);
    int lastFinish = sortedCourses[0].finish;
    for (std::size_t i = 1; i < sortedCourses.size(); ++i) {
        if (sortedCourses[i].start >= lastFinish) {
            selected.push_back(sortedCourses[i]);
            lastFinish = sortedCourses[i].finish;
        }
    }
    return selected;
}

std::vector<Course> selectCoursesDP(
    const std::vector<Course>& sortedCourses) {
    const std::size_t n = sortedCourses.size();
    if (n == 0) {
        return {};
    }

    std::vector<int> previous(n, -1);
    for (std::size_t j = 0; j < n; ++j) {
        for (int i = static_cast<int>(j) - 1; i >= 0; --i) {
            if (sortedCourses[static_cast<std::size_t>(i)].finish <=
                sortedCourses[j].start) {
                previous[j] = i;
                break;
            }
        }
    }

    std::vector<long long> dp(n, 0);
    for (std::size_t j = 0; j < n; ++j) {
        long long includeValue = sortedCourses[j].enrollments +
            (previous[j] == -1
                 ? 0LL
                 : dp[static_cast<std::size_t>(previous[j])]);
        long long excludeValue = j == 0
            ? 0LL
            : dp[j - 1];
        dp[j] = includeValue > excludeValue ? includeValue : excludeValue;
    }

    std::vector<Course> selected;
    int j = static_cast<int>(n) - 1;
    while (j >= 0) {
        std::size_t index = static_cast<std::size_t>(j);
        long long includeValue = sortedCourses[index].enrollments +
            (previous[index] == -1
                 ? 0LL
                 : dp[static_cast<std::size_t>(previous[index])]);
        long long excludeValue = index == 0 ? 0LL : dp[index - 1];

        if (includeValue > excludeValue) {
            selected.push_back(sortedCourses[index]);
            j = previous[index];
        } else {
            --j;
        }
    }

    std::reverse(selected.begin(), selected.end());
    return selected;
}

std::vector<std::vector<Course>> allocateClassrooms(
    const std::vector<Course>& courses) {
    if (courses.empty()) {
        return {};
    }

    std::vector<Course> sorted = courses;
    mergeSortWithComparator(sorted, compareByStart);

    std::vector<std::vector<Course>> classrooms;
    std::vector<int> lastFinishTimes;
    for (const Course& course : sorted) {
        bool foundRoom = false;
        std::size_t bestRoom = 0;
        int earliestFinish = 0;

        for (std::size_t room = 0; room < classrooms.size(); ++room) {
            if (lastFinishTimes[room] <= course.start) {
                if (!foundRoom || lastFinishTimes[room] < earliestFinish) {
                    foundRoom = true;
                    bestRoom = room;
                    earliestFinish = lastFinishTimes[room];
                }
            }
        }

        if (foundRoom) {
            classrooms[bestRoom].push_back(course);
            lastFinishTimes[bestRoom] = course.finish;
        } else {
            classrooms.push_back({course});
            lastFinishTimes.push_back(course.finish);
        }
    }
    return classrooms;
}

std::size_t optimalCountExhaustive(
    const std::vector<Course>& courses) {
    std::size_t bestCount = 0;
    const std::size_t totalMasks = std::size_t(1) << courses.size();
    for (std::size_t mask = 0; mask < totalMasks; ++mask) {
        if (!isSubsetValid(courses, mask)) {
            continue;
        }
        std::size_t count = 0;
        for (std::size_t i = 0; i < courses.size(); ++i) {
            if ((mask & (std::size_t(1) << i)) != 0) {
                ++count;
            }
        }
        if (count > bestCount) {
            bestCount = count;
        }
    }
    return bestCount;
}

std::size_t minClassroomsExhaustive(
    const std::vector<Course>& courses) {
    if (courses.empty()) {
        return 0;
    }
    std::vector<std::vector<Course>> rooms;
    std::size_t bestRooms = courses.size();
    searchMinClassrooms(courses, 0, rooms, bestRooms);
    return bestRooms;
}

long long optimalEnrollmentsExhaustive(
    const std::vector<Course>& courses) {
    long long bestEnrollments = 0;
    const std::size_t totalMasks = std::size_t(1) << courses.size();
    for (std::size_t mask = 0; mask < totalMasks; ++mask) {
        if (!isSubsetValid(courses, mask)) {
            continue;
        }
        long long total = 0;
        for (std::size_t i = 0; i < courses.size(); ++i) {
            if ((mask & (std::size_t(1) << i)) != 0) {
                total += courses[i].enrollments;
            }
        }
        if (total > bestEnrollments) {
            bestEnrollments = total;
        }
    }
    return bestEnrollments;
}
