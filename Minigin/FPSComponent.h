
class FpsComponent {
public:
	FpsComponent() = default;
	void StartLoop();
	void EndLoop();
	float GetFps();
private:
	float m_LastFps;
	long long m_StartLoopTime;
	long long m_EndLoopTime;
};