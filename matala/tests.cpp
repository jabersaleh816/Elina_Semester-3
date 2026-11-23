#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Algorithms.h"

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace {

long long sumEnrollments(const std::vector<Course>& courses) {
    long long total = 0;
    for (const Course& course : courses) {
        total += course.enrollments;
    }
    return total;
}

bool sameCourse(const Course& a, const Course& b) {
    return a.id == b.id &&
           a.start == b.start &&
           a.finish == b.finish &&
           a.enrollments == b.enrollments;
}

bool hasCourse(const std::vector<Course>& courses, const Course& expected) {
    for (const Course& course : courses) {
        if (sameCourse(course, expected)) {
            return true;
        }
    }
    return false;
}

bool coursesOverlap(const Course& a, const Course& b) {
    return !(a.finish <= b.start || b.finish <= a.start);
}

bool isValidSelection(const std::vector<Course>& input,
                      const std::vector<Course>& selected) {
    for (const Course& course : selected) {
        if (!hasCourse(input, course)) {
            return false;
        }
    }
    for (std::size_t i = 0; i < selected.size(); ++i) {
        if (i > 0 && selected[i].start < selected[i - 1].finish) {
            return false;
        }
        for (std::size_t j = i + 1; j < selected.size(); ++j) {
            if (coursesOverlap(selected[i], selected[j])) {
                return false;
            }
        }
    }
    return true;
}

bool isValidRoomAllocation(const std::vector<Course>& input,
                           const std::vector<std::vector<Course>>& rooms) {
    if (input.empty()) {
        return rooms.empty();
    }
    if (rooms.empty()) {
        return false;
    }

    for (const std::vector<Course>& room : rooms) {
        if (room.empty()) {
            return false;
        }
        for (std::size_t i = 0; i < room.size(); ++i) {
            if (i > 0 && room[i].start < room[i - 1].finish) {
                return false;
            }
            for (std::size_t j = i + 1; j < room.size(); ++j) {
                if (coursesOverlap(room[i], room[j])) {
                    return false;
                }
            }
        }
    }

    for (const Course& original : input) {
        std::size_t occurrences = 0;
        for (const std::vector<Course>& room : rooms) {
            for (const Course& allocated : room) {
                if (sameCourse(original, allocated)) {
                    ++occurrences;
                }
            }
        }
        if (occurrences != 1) {
            return false;
        }
    }
    return true;
}

std::uint32_t nextRandom(std::uint32_t& state) {
    state = state * 1664525u + 1013904223u;
    return state;
}

std::vector<Course> generateRandomCourses(std::size_t n,
                                          std::uint32_t& state) {
    std::vector<Course> courses;
    courses.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        int start = static_cast<int>(nextRandom(state) % 21);
        int duration = 1 + static_cast<int>(nextRandom(state) % 8);
        long long enrollments =
            1 + static_cast<long long>(nextRandom(state) % 20);
        courses.push_back({
            "C" + std::to_string(i),
            start,
            start + duration,
            enrollments
        });
    }
    return courses;
}

std::string readTextFile(const std::string& path) {
    std::ifstream input(path);
    if (!input) {
        return {};
    }
    std::ostringstream buffer;
    buffer << input.rdbuf();
    return buffer.str();
}

} // namespace

TEST_CASE("merge sort applies the exact COUNT and ENROLLMENTS tie-breakers") {
    std::vector<Course> courses = {
        {"Zulu", 540, 600, 1},
        {"Alpha", 500, 600, 2},
        {"Beta", 520, 600, 3}
    };

    mergeSort(courses);

    REQUIRE(courses.size() == 3);
    CHECK(courses[0].id == "Alpha");
    CHECK(courses[1].id == "Beta");
    CHECK(courses[2].id == "Zulu");
}

TEST_CASE("sample case") {
    std::vector<Course> original = {
        {"Algorithms", 540, 600, 4},
        {"Databases", 600, 660, 4},
        {"Operating_Systems", 540, 660, 11}
    };
    std::vector<Course> sorted = original;
    mergeSort(sorted);

    std::vector<Course> count = selectCourses(sorted);
    std::vector<Course> enrollment = selectCoursesDP(sorted);
    std::vector<std::vector<Course>> rooms = allocateClassrooms(original);

    REQUIRE(count.size() == 2);
    CHECK(count[0].id == "Algorithms");
    CHECK(count[1].id == "Databases");
    CHECK(sumEnrollments(count) == 8);

    REQUIRE(rooms.size() == 2);
    CHECK(rooms[0].size() == 2);
    CHECK(rooms[1].size() == 1);

    REQUIRE(enrollment.size() == 1);
    CHECK(enrollment[0].id == "Operating_Systems");
    CHECK(sumEnrollments(enrollment) == 11);
    CHECK(readTextFile("testdata/sample.in").find("Operating_Systems") != std::string::npos);
    CHECK(readTextFile("testdata/sample.out").find("ENROLLMENTS") != std::string::npos);
}

TEST_CASE("empty case") {
    std::vector<Course> empty;
    std::vector<Course> sorted = empty;
    mergeSort(sorted);

    CHECK(selectCourses(sorted).empty());
    CHECK(selectCoursesDP(sorted).empty());
    CHECK(allocateClassrooms(empty).empty());
    CHECK(optimalCountExhaustive(empty) == 0);
    CHECK(minClassroomsExhaustive(empty) == 0);
    CHECK(optimalEnrollmentsExhaustive(empty) == 0);
    CHECK(readTextFile("testdata/empty.in") == "0\n");
    CHECK(readTextFile("testdata/empty.out").find("ROOMS\n0") != std::string::npos);
}

TEST_CASE("tie case") {
    std::vector<Course> original = {
        {"Databases", 540, 600, 5},
        {"Algorithms", 540, 600, 5},
        {"Operating_Systems", 600, 660, 2}
    };
    std::vector<Course> sorted = original;
    mergeSort(sorted);

    std::vector<Course> count = selectCourses(sorted);
    std::vector<Course> enrollment = selectCoursesDP(sorted);
    std::vector<std::vector<Course>> rooms = allocateClassrooms(original);

    REQUIRE(count.size() == 2);
    CHECK(count[0].id == "Algorithms");
    CHECK(count[1].id == "Operating_Systems");
    CHECK(readTextFile("testdata/ties.in").find("Operating_Systems") != std::string::npos);
    CHECK(readTextFile("testdata/ties.out").find("2 7") != std::string::npos);

    REQUIRE(enrollment.size() == 2);
    CHECK(enrollment[0].id == "Algorithms");
    CHECK(enrollment[1].id == "Operating_Systems");
    REQUIRE(rooms.size() == 2);
    CHECK(rooms[0][0].id == "Algorithms");
    CHECK(rooms[0][1].id == "Operating_Systems");
    CHECK(rooms[1][0].id == "Databases");
}

TEST_CASE("single course") {
    std::vector<Course> courses = {
        {"Single", 100, 200, 17}
    };
    std::vector<Course> sorted = courses;
    mergeSort(sorted);

    CHECK(selectCourses(sorted).size() == 1);
    CHECK(selectCoursesDP(sorted).size() == 1);
    CHECK(sumEnrollments(selectCoursesDP(sorted)) == 17);
    CHECK(minClassroomsExhaustive(courses) == 1);
}

TEST_CASE("touching endpoints can share a classroom") {
    std::vector<Course> courses = {
        {"First", 0, 10, 3},
        {"Second", 10, 20, 4}
    };

    std::vector<Course> sorted = courses;
    mergeSort(sorted);
    CHECK(selectCourses(sorted).size() == 2);

    std::vector<std::vector<Course>> rooms =
        allocateClassrooms(courses);
    REQUIRE(rooms.size() == 1);
    CHECK(rooms[0].size() == 2);
}

TEST_CASE("all courses overlapping require separate classrooms") {
    std::vector<Course> courses = {
        {"A", 0, 30, 5},
        {"B", 5, 25, 8},
        {"C", 10, 20, 13}
    };

    CHECK(minClassroomsExhaustive(courses) == 3);
    std::vector<std::vector<Course>> rooms =
        allocateClassrooms(courses);
    CHECK(rooms.size() == 3);
    CHECK(isValidRoomAllocation(courses, rooms));
}

TEST_CASE("identical times use lexicographical IDs for finishing sort") {
    std::vector<Course> courses = {
        {"Zulu", 540, 600, 1},
        {"Alpha", 540, 600, 2},
        {"Mike", 540, 600, 3}
    };

    mergeSort(courses);
    CHECK(courses[0].id == "Alpha");
    CHECK(courses[1].id == "Mike");
    CHECK(courses[2].id == "Zulu");
    CHECK(selectCourses(courses).size() == 1);
}

TEST_CASE("unsorted input is handled by each result") {
    std::vector<Course> courses = {
        {"Late", 20, 30, 30},
        {"Early", 0, 5, 2},
        {"Middle", 5, 20, 7}
    };

    std::vector<Course> byFinish = courses;
    mergeSort(byFinish);
    CHECK(selectCourses(byFinish).size() == 3);
    CHECK(sumEnrollments(selectCoursesDP(byFinish)) == 39);
    CHECK(minClassroomsExhaustive(courses) == 1);
}

TEST_CASE("gap between maximum count and maximum enrollments") {
    std::vector<Course> courses = {
        {"ShortA", 0, 10, 1},
        {"ShortB", 10, 20, 1},
        {"Long", 5, 15, 100}
    };
    std::vector<Course> sorted = courses;
    mergeSort(sorted);

    CHECK(selectCourses(sorted).size() == 2);
    CHECK(optimalCountExhaustive(courses) == 2);
    CHECK(sumEnrollments(selectCoursesDP(sorted)) == 100);
    CHECK(optimalEnrollmentsExhaustive(courses) == 100);
}

TEST_CASE("DP reconstruction excludes a course on an exact tie") {
    std::vector<Course> courses = {
        {"Wide", 0, 20, 5},
        {"Short", 0, 10, 5}
    };
    mergeSort(courses);

    std::vector<Course> selected = selectCoursesDP(courses);
    REQUIRE(selected.size() == 1);
    CHECK(selected[0].id == "Short");
    CHECK(sumEnrollments(selected) == 5);
}

TEST_CASE("classrooms are reused before a new classroom opens") {
    std::vector<Course> courses = {
        {"A", 0, 10, 1},
        {"B", 10, 20, 1},
        {"C", 20, 30, 1}
    };

    std::vector<std::vector<Course>> rooms =
        allocateClassrooms(courses);
    REQUIRE(rooms.size() == 1);
    CHECK(rooms[0].size() == 3);
}

TEST_CASE("Buggy p[j] predecessor regression") {
    std::vector<Course> courses = {
        {"A", 0, 2, 9},
        {"B", 3, 10, 2},
        {"C", 5, 12, 10}
    };
    mergeSort(courses);

    std::size_t j = 2;
    int brokenPrevious = -1;
    if (courses[j - 1].finish <= courses[j].start) {
        brokenPrevious = static_cast<int>(j - 1);
    }
    int correctPrevious = -1;
    for (std::size_t i = j; i > 0; --i) {
        if (courses[i - 1].finish <= courses[j].start) {
            correctPrevious = static_cast<int>(i - 1);
            break;
        }
    }

    CHECK(brokenPrevious == -1);
    CHECK(correctPrevious == 0);
    CHECK(courses[0].enrollments + courses[2].enrollments >
          courses[2].enrollments);
    CHECK(optimalEnrollmentsExhaustive(courses) == 19);
    CHECK(sumEnrollments(selectCoursesDP(courses)) == 19);
}

TEST_CASE("exhaustive comparisons for every input through five courses") {
    std::uint32_t state = 0xC0FFEEu;
    for (std::size_t n = 0; n <= 5; ++n) {
        for (int sample = 0; sample < 80; ++sample) {
            std::vector<Course> courses =
                generateRandomCourses(n, state);

            std::vector<Course> sortedByFinish = courses;
            mergeSort(sortedByFinish);
            std::vector<Course> count =
                selectCourses(sortedByFinish);
            std::vector<Course> enrollment =
                selectCoursesDP(sortedByFinish);
            std::vector<std::vector<Course>> rooms =
                allocateClassrooms(courses);

            CHECK(count.size() ==
                  optimalCountExhaustive(courses));
            CHECK(sumEnrollments(enrollment) ==
                  optimalEnrollmentsExhaustive(courses));
            CHECK(rooms.size() ==
                  minClassroomsExhaustive(courses));
            CHECK(isValidSelection(courses, count));
            CHECK(isValidSelection(courses, enrollment));
            CHECK(isValidRoomAllocation(courses, rooms));
        }
    }
}

TEST_CASE("structural validity for larger unsorted inputs") {
    std::vector<Course> courses = {
        {"A", 0, 12, 5},
        {"B", 2, 4, 7},
        {"C", 4, 8, 11},
        {"D", 8, 16, 13},
        {"E", 10, 11, 17},
        {"F", 11, 20, 19},
        {"G", 15, 18, 23},
        {"H", 18, 25, 29}
    };

    std::vector<Course> sorted = courses;
    mergeSort(sorted);
    std::vector<Course> count = selectCourses(sorted);
    std::vector<Course> enrollment = selectCoursesDP(sorted);
    std::vector<std::vector<Course>> rooms =
        allocateClassrooms(courses);

    CHECK(isValidSelection(courses, count));
    CHECK(isValidSelection(courses, enrollment));
    CHECK(isValidRoomAllocation(courses, rooms));
}
