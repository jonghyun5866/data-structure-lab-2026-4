#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

//Music Class
class Music {
private:
	string title;
	string artist;
	string albm;
	int year;
public:
	//생성자
	Music(string title, string artist, string album, int year) {
		this->title = title;
		this->artist = artist;
		this->albm = album;
		this->year = year;
	}
	// Getters
	string getTitle() { return title; }
	string getArtist() { return artist; }
	string getAlbum() { return albm; }
	int getYear() { return year; }
};

// MusicStreamingService Class
class MusicStreamingService {
private:
	string serviceName;
	vector<Music> musicList;
public:
	MusicStreamingService(string serviceName) {
		this->serviceName = serviceName;
	}
	// Music 객체를 service에 추가
	void addMusic(string title, string artist, string album, int year) {
		Music newMusic(title, artist, album, year);
		musicList.push_back(newMusic);
		cout << title << "by " << artist << " addede to " << serviceName << endl;
	}
	//TITLE로 검색하기
	Music* searchByTitle(string title) {
		for (int i = 0; i < musicList.size(); i++) {
			if (musicList[i].getTitle() == title)
				return &musicList[i];
		}
		return NULL;
	}
	// Artist로 검색하기
	vector<Music*> searchByArtist(string artist) {
		vector<Music*> result;
		//musicList에 있는 artist와 입력 artist가 같으면 result에 push_back
		for (int i = 0; i < musicList.size(); i++) {
			if (musicList[i].getArtist() == artist) {
				result.push_back(&musicList[i]);
			}
		}
		return result;
	}
};