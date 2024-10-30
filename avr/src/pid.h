class PIDController {
public:
    PIDController(float kp, float ki, float kd);
    float compute(float setpoint, float measured_angle, float dt);

private:
    float kp_, ki_, kd_;
    float prev_error_;
    float integral_;
};