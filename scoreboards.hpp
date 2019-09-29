#include <array>
#include <string>
#include <string_view>
#include <utility>
#include <tuple>
#include <map>
#include <vector>

#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include <QString>

constexpr std::array upper = {"1", "2", "3", "4", "5", "6"};
constexpr std::array lower = {"P", "2P", "Mst", "Dst", "T", "F", "K", "P", "Sz"};

class gridder
{
public:

    static QGridLayout* layout_;
    static QWidget* parent_;
    static int row_;
    static int col_;

    static void set(QGridLayout* layout, QWidget* parent = nullptr)
    {
        layout_ = layout;
        parent_ = parent;
        row_ = 0;
        col_ = 0;
    }

    template <typename TQWidget, typename ... TArgs>
    static TQWidget* add(TArgs&& ... args)
    {
        auto widget = new TQWidget{ std::forward<TArgs>(args) ..., parent_ };
        layout_->addWidget(widget, row_++, col_, Qt::AlignRight);
        return widget;
    }

    static void new_column()
    {
        row_ = 0;
        ++col_;
    }
};

inline QLineEdit* scorecell()
{
    auto line_edit = gridder::add<QLineEdit>();
    line_edit->setFixedWidth(20);
    return line_edit;
}

inline QLineEdit* scoring_section(const std::vector<QLineEdit*>& scores, bool upper = false)
{
    auto label = gridder::add<QLineEdit>();
    label->setDisabled(true);
    for (const auto& e : scores)
    {
        QObject::connect(e, &QLineEdit::textChanged, [scores, label, upper](auto) {
            const int score_sum = std::accumulate(scores.begin(), scores.end(), 0, [](int sum, QLineEdit* edit){
                return sum + edit->text().toInt();
            });
            label->setText(QString::number(score_sum + (upper ? 50 * ((score_sum > 0) - (score_sum < 0)) : 0)));
        });
    }
    return label;
}

inline QLineEdit* column(const std::string& name, QLineEdit* previous = nullptr)
{
    gridder::add<QLabel>(QString::fromStdString(name));
    std::vector<QLineEdit*> upper_section;
    for (const auto& e : upper) upper_section.push_back(gridder::add<QLineEdit>());
    auto upper_label = scoring_section(upper_section, true);

    std::vector<QLineEdit*> lower_section;
    for (const auto& e : lower) lower_section.push_back(gridder::add<QLineEdit>());
    lower_section.push_back(upper_label);
    if (previous) lower_section.push_back(previous);
    auto lower_label = scoring_section(lower_section);
    gridder::new_column();
    return lower_label;
}

inline void construct_scoreboard()
{
    gridder::add<QLabel>();
    for (const auto& e : upper) gridder::add<QLabel>(e);
    gridder::add<QLabel>();
    for (const auto& e : lower) gridder::add<QLabel>(e);
    gridder::new_column();

    auto l_first = column("L");
    auto m_first = column("M");
    auto l_second = column("L", l_first);
    auto m_second = column("M", m_first);
    auto l_third = column("L", l_second);
    auto m_third = column("M", m_second);
}